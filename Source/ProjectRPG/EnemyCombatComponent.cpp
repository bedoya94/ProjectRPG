// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "EnemyCombatComponent.h"
#include "Enemy.h"
#include "ProjectRPGCharacter.h"


// Sets default values for this component's properties
UEnemyCombatComponent::UEnemyCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("My_Weapon"));
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("My_HitBox"));
	HitBox->AttachTo(WeaponMesh);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &UEnemyCombatComponent::HitCollision);
	attackCoolDown = 5.f;
	attackTimer = 0.f;
	bCanAttack = false;
	bAttacking = false;
	bAttack = false;
}


// Called when the game starts
void UEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEnemy>(GetOwner());
	// ...
	
}


// Called every frame
void UEnemyCombatComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (Owner)
	{
		Target = Cast<AProjectRPGCharacter>(Owner->Player);

		if (attackTimer <= 0.f)
		{
			attackTimer = 0.f;
			bCanAttack = true;
		}

		if (attackTimer > 0.f)
		{
			attackTimer -= DeltaTime;
			bCanAttack = false;
		}

		if (!Owner->bStaggered)
			OnAttacking();
	}
}

void UEnemyCombatComponent::OnAttacking()
{
	if (Owner->Player)
	{
		float dist = FVector::Dist(Target->GetActorLocation(), Owner->GetActorLocation());

		if ((Owner->PlayerInSightAndDist(150.f)) && bCanAttack)
		{	
			ThingsHit.Empty();
			Owner->PlayAnimMontage(AttackMontages[0]);
			attackTimer = attackCoolDown;
			bAttack = true;
			bCanAttack = false;
		}

	}
}

void UEnemyCombatComponent::ResetAttack()
{
	ThingsHit.Empty();
	bAttack = false;
	bCanAttack = false;
	bAttacking = false;
	attackTimer = attackCoolDown;
}

void UEnemyCombatComponent::HitCollision_Implementation(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bAttacking && OtherActor != Owner && !ThingsHit.Contains(OtherActor))
	{
		ThingsHit.Add(OtherActor);

		AProjectRPGCharacter* const Target = Cast<AProjectRPGCharacter>(OtherActor);

		if (Target)
		{
			Target->bHitted = true;
			Target->takenDanage = DealDamage();
		}
	}
}

float UEnemyCombatComponent::DealDamage()
{
	float baseDamage = Owner->strenght;
	float totalDamage = 0.f;

	return totalDamage + baseDamage;
}