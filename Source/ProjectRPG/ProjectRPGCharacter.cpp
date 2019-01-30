// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ProjectRPG.h"
#include "ProjectRPGCharacter.h"
#include "MeleeWeapon.h"
//////////////////////////////////////////////////////////////////////////
// AProjectRPGCharacter

AProjectRPGCharacter::AProjectRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	bCanRun = false;
	bCanMove = true;
	zoomFactor = 0.f;

	CombatComp = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("MyCombatComponent"));
	DefenseComp = CreateDefaultSubobject<UPlayerDefenseComponent>(TEXT("MyDefenseComponent"));
	TargetComp = CreateDefaultSubobject<UTargetingComponent>(TEXT("MyTargetingComponent"));
	
	strenght = 5.f;
	defense = 10.f;
	MaxHP = 100.f;
	MaxSP = 50.f;
	spCost = 0.f;
	
	bCanSPRegain = true;
	bHitted = false;
	curXp = 0.f;
	totalXp = 0;
	xpToLvl = 100;
	levelNum = 1;
}

void AProjectRPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->AddOwnedComponent(CombatComp);
	this->AddOwnedComponent(DefenseComp);
	this->AddOwnedComponent(TargetComp);

	if(TargetComp)
		TargetComp->TargetingSphere->AttachTo(GetRootComponent());

	if (BPMeleeWeapon)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("R_HandSocket");
		FRotator SocketRot = GetMesh()->GetSocketRotation("R_HandSocket");

		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());
	}
}

void AProjectRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	HitPoints = MaxHP;
	StaminaPoints = MaxSP;
}

void AProjectRPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HitPoints <= 0.f)
		return;

	GLog->Log("Hello World!");

	{
		if (GetCharacterMovement()->Velocity.IsZero())
		{
			bCanRun = false;
		}

		if (bCanRun)
		{
			GetCharacterMovement()->MaxWalkSpeed = 700.f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}

		if (!CombatComp->bCanAttack && DefenseComp->dodgeTimer == 0.f)
			bCanMove = true;
	}

	if (bInCombat)
	{
		zoomFactor += DeltaSeconds / 0.5f;
	}
	else
	{
		zoomFactor -= DeltaSeconds / 0.5f;
	}

	{
		SetSPCost();
	
		if (bCanSPRegain && DefenseComp->dodgeTimer <= 0.f && !bCanRun && !DefenseComp->bCountering)
		{
			float spRecoverRate = 0.f;

			if (!DefenseComp->bDefending)
				spRecoverRate = 25.f;
			else
				spRecoverRate = 5.f;

			if (StaminaPoints < MaxSP)
			{
				StaminaPoints += (spRecoverRate * DeltaSeconds);
			}
			else
			{
				StaminaPoints = MaxSP;
			}	
		}
	}

	{
		if (bHitted)
		{
			if (!DefenseComp->bDefending && !DefenseComp->bCountering)
			{
				if (takenDanage >+defense)
					HitPoints -= (takenDanage - defense);
				else
					HitPoints -= 1.f;
			}
			else if (DefenseComp->bCanCounter)
			{
				StaminaPoints += 15.f;
				DefenseComp->bCountering = true;
				PlayAnimMontage(DefenseComp->DefenseAnims[FMath::RandRange(2,3)]);
				DefenseComp->bCanCounter = false;
				CombatComp->bHeavyAttack = true;
			}
			else
			{
				StaminaPoints -= 15.f;
			}
		
			bHitted = false;
		}
	}

	if (DefenseComp->bCountering)
		bCanMove = false;

	XpManager();

	zoomFactor = FMath::Clamp(zoomFactor, 0.f, 1.f);
	GetCameraBoom()->TargetArmLength = FMath::Lerp(150.f, 300.f, zoomFactor);
	GetCameraBoom()->SocketOffset.Y = FMath::Lerp(50.f, 90.f, zoomFactor);
	GetCameraBoom()->SocketOffset.Z = FMath::Lerp(25.f, 30.f, zoomFactor);
	LookVector = GetController()->GetActorForwardVector();
}
//////////////////////////////////////////////////////////////////////////
// Input

void AProjectRPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	//InputComponent->BindAction("Jump", IE_Pressed, this, &AProjectRPGCharacter::RollDodge);

	InputComponent->BindAxis("Attacking",this, &AProjectRPGCharacter::OnAttackMode);
	InputComponent->BindAction("Attacking", IE_Released, this, &AProjectRPGCharacter::ExitAttackMode);

	InputComponent->BindAction("TargetUp", IE_Pressed, this, &AProjectRPGCharacter::TargetIndexUp);
	InputComponent->BindAction("TargetDown", IE_Pressed, this, &AProjectRPGCharacter::TargetIndexDown);
	InputComponent->BindAction("LockOnOff", IE_Pressed, this, &AProjectRPGCharacter::LockOnEnemy);

	InputComponent->BindAxis("Defense", this, &AProjectRPGCharacter::OnDefenseMode);
	//InputComponent->BindAction("Defense", IE_Released, this, &AProjectRPGCharacter::ExitDefenseMode);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AProjectRPGCharacter::OnAttackMode(float Value)
{
	if (CombatComp)
		CombatComp->Attacking(Value);
}

void AProjectRPGCharacter::ExitAttackMode()
{
	if (CombatComp)
		CombatComp->StopAttacking();
}

void AProjectRPGCharacter::OnDefenseMode(float Value)
{
	if (DefenseComp)
		DefenseComp->OnDefending(Value);
}

void AProjectRPGCharacter::ExitDefenseMode()
{
	if (DefenseComp)
		DefenseComp->StopDefending();
}

void AProjectRPGCharacter::Launcher()
{
	LaunchCharacter(GetActorForwardVector() * launchForce, false, false);
}

void AProjectRPGCharacter::TargetIndexUp()
{
	if(TargetComp)
		TargetComp->IndexUp();
}

void AProjectRPGCharacter::TargetIndexDown()
{
	if (TargetComp)
		TargetComp->IndexDown();
}

void AProjectRPGCharacter::LockOnEnemy()
{
	if (TargetComp)
	{
		TargetComp->LockOnTarget();
	}
}

float AProjectRPGCharacter::SetSPCost()
{
	if (!DefenseComp->bCountering)
	{
		if (DefenseComp->dodgeTimer > 0.f)
		{
			spCost = 10.f;
		}
		else
		{
			if (CombatComp->bCanAttack)
			{
				if (CombatComp->stringAttackNum < 5)
				{
					spCost = 10.f;
				}
				else
				{
					if (!DefenseComp->bCountering)
						spCost = 20.f;
					else
						spCost = 1.f;
				}
			}
		}
	}
	else
	{
		spCost = 0.f;
	}


	float _spCost = spCost;
	return _spCost;
}

void AProjectRPGCharacter::StaminaExpended()
{
	bCanSPRegain = false;
	StaminaPoints -= SetSPCost();
}

void AProjectRPGCharacter::XpManager()
{
	totalXp = FMath::RoundToInt(curXp);

	if (totalXp >= xpToLvl)
		bLevelUp = true;

		if (bLevelUp)
		{
			levelNum++;
			xpToLvl *= levelNum;
			MaxHP += 100.f;
			MaxSP += 50.f;
			HitPoints = MaxHP;
			StaminaPoints = MaxSP;

			if (strenght <= 255)
				strenght += 5.f;
			else
				strenght = 255;

			if (defense <= 255)
				defense += 5.f;
			else
				defense = 255;

			baseAttack = strenght;
			baseDefense = defense;
			bLevelUp = false;
		}
}