// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "MeleeWeapon.h"


// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bSwinging = false;
	WeaponHolder = NULL;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Component"));
	SceneComp->AttachTo(RootComponent);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	HitBox->AttachTo(SceneComp);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnHit);
}

void AMeleeWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponHolder = Cast<AProjectRPGCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AMeleeWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//AttachWeaponToHolder();

	if (WeaponHolder->bInCombat)
	{
		AttachRootComponentTo(WeaponHolder->GetMesh(), FName(TEXT("R_HandSocket")), EAttachLocation::SnapToTargetIncludingScale);
	}
	else
	{
		AttachRootComponentTo(WeaponHolder->GetMesh(), FName(TEXT("DrawWeaponSocket")), EAttachLocation::SnapToTargetIncludingScale);
	}


}

void AMeleeWeapon::OnHit_Implementation(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const  FHitResult &SweepResult)
{
	if (bSwinging && OtherActor != WeaponHolder && !ThingsHit.Contains(OtherActor))
	{
		ThingsHit.Add(OtherActor);
		
		AEnemy* const Target = Cast<AEnemy>(OtherActor);

		if (Target)
		{
			Target->bHitted = true;
		}
	}
}

void AMeleeWeapon::Swing()
{
	ThingsHit.Empty();
	bSwinging = true;
}

void AMeleeWeapon::Rest()
{
	ThingsHit.Empty();
	bSwinging = false;
}

void AMeleeWeapon::AttachWeaponToHolder()
{
	if (WeaponHolder)
	{
		SetOwner(WeaponHolder);

		USkeletalMeshComponent* ArmMesh = WeaponHolder->GetMesh();

		if (ArmMesh)
		{
			if (!WeaponHolder->bInCombat && !WeaponHolder->CombatComp->bCanAttack && !WeaponHolder->DefenseComp->bDefending)
			{
				AttachRootComponentTo(ArmMesh, FName(TEXT("DrawWeaponSocket")), EAttachLocation::SnapToTarget);
			}
			else if(WeaponHolder->bInCombat || WeaponHolder->CombatComp->bCanAttack || WeaponHolder->DefenseComp->bDefending)
			{
				AttachRootComponentTo(ArmMesh, FName(TEXT("R_HandSocket")), EAttachLocation::SnapToTarget);
			}
		}
	}
}