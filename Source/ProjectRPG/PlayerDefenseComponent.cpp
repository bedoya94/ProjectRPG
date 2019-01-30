// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "PlayerDefenseComponent.h"
#include "ProjectRPGCharacter.h"


// Sets default values for this component's properties
UPlayerDefenseComponent::UPlayerDefenseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	bCanDodge = false;
	dodgeCoolDown = 1.2f;
	dodgeTimer = 0.f;
	bIsDodging = false;
	bCanCounter = false;
	counterTimer = 0.f;
	counterLimit = 0.25f;
}


// Called when the game starts
void UPlayerDefenseComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = Cast<AProjectRPGCharacter>(GetOwner());
	
}


// Called every frame
void UPlayerDefenseComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	{
		if (bDefending)
		{
			MyOwner->GetCharacterMovement()->bOrientRotationToMovement = false;

			if (MyOwner->GetCharacterMovement()->Velocity.IsZero())
				MyOwner->bUseControllerRotationYaw = false;
			else
				MyOwner->bUseControllerRotationYaw = true;
		}
		else
		{
			MyOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
			MyOwner->GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
			MyOwner->bUseControllerRotationPitch = false;
			MyOwner->bUseControllerRotationYaw = false;
			MyOwner->bUseControllerRotationRoll = false;

		}
	}

	{
		if (dodgeTimer <= 0.f)
		{
			bIsDodging = false;

			dodgeTimer = 0.f;
			bCanDodge = true;
		}

		if (dodgeTimer > 0.f)
		{
			MyOwner->launchForce = 25.f;
			MyOwner->bCanMove = false;
			MyOwner->bUseControllerRotationYaw = false;
			bDefending = false;
			bCanDodge = false;
			MyOwner->bCanSPRegain = false;
			dodgeTimer -= DeltaTime;
		}

		if (dodgeTimer >= dodgeCoolDown - 0.1f)
		{
			bIsDodging = true;
			MyOwner->CombatComp->AttackReset();
			MyOwner->bCanMove = true;
			MyOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
			MyOwner->GetCharacterMovement()->RotationRate = FRotator(0.0f, 5400.0f, 0.0f);
			if(MyOwner)
				MyOwner->PlayAnimMontage(DefenseAnims[1]);
		}

		if (bIsDodging)
		{
			MyOwner->Launcher();
		}
	}
}

void UPlayerDefenseComponent::OnDefending(float Value)
{
	GLog->Log("Counter Timer = " + FString::SanitizeFloat(counterTimer));

	if (Value > 0.f && !bIsDodging && MyOwner->StaminaPoints >= 0.f && !bCountering)
	{
		MyOwner->CombatComp->AttackReset();
		bDefending = true;
		bCanCounter = true;
		counterTimer += GetWorld()->GetDeltaSeconds();

		if (counterTimer >= counterLimit)
		{
			bCanCounter = false;
			counterTimer = counterLimit;
		}
			
	}
	else
	{
		bCanCounter = false;
		bDefending = false;
		counterTimer = 0.f;
	}	
}

void UPlayerDefenseComponent::StopDefending()
{
	bDefending = false;
}

void UPlayerDefenseComponent::RollDodge()
{
	if (bCanDodge && MyOwner->StaminaPoints >= 0.f && !MyOwner->GetCharacterMovement()->IsFalling())
	{
		bIsDodging = true;
		dodgeTimer = dodgeCoolDown;
	}
}