// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "Enemy.h"
#include "ProjectRPGCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthPoints = 0.f;
	dir = 0.f;
	bHitted = false;
	xp = 0.f;
	bStaggered = false;
	stagTimer = 0.f;
	stagCoolDown = 2.f;

	strenght = 0.f;
	defense = 0.f;

	CombatComp = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("My_CombatComponent"));
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->AddOwnedComponent(CombatComp);
	CombatComp->WeaponMesh->AttachTo(GetMesh(), "RightHandSocket", EAttachLocation::SnapToTarget);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AProjectRPGCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (HealthPoints <= 0.f)
	{
		Player->TargetComp->CollectedEnemies.Remove(Player->TargetComp->CurrentTarget);
		
		if (Player->TargetComp->TargetIndex >= Player->TargetComp->CollectedEnemies.Num())
			Player->TargetComp->TargetIndex = Player->TargetComp->CollectedEnemies.Num() - 1;

		Player->curXp += xp;
		Destroy();
	}

	dir = DirectionToPlayer();

	if (Player)
	{
		if (Player->TargetComp->CurrentTarget == this)
		{
			bLockedOn = true;
		}
		else
		{
			bLockedOn = false;
		}

		if (bStaggered)
		{
			stagTimer += DeltaTime;
			if (stagTimer >= stagCoolDown)
				bStaggered = false;
		}
		else
		{
			stagTimer = 0.f;
		}

		if (bHitted)
		{
			if (Player->DefenseComp->bCountering)
				bStaggered = true;

			if (Player->CombatComp->DealDamage() >= defense)
				HealthPoints -= (Player->CombatComp->DealDamage() - defense);
			else
				HealthPoints -= 1.f;

			bHitted = false;
		}
	}

	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

float AEnemy::DirectionToPlayer()
{
	FVector Direction = GetActorLocation() - Player->GetActorLocation();
	Direction.Normalize();
	float dir = FVector::DotProduct(Direction, Player->LookVector);

	return dir;
}

bool AEnemy::PlayerInSightAndDist(float limitDist)
{
	float dist = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	FVector Direction = Player->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	float dir = FVector::DotProduct(Direction, GetActorForwardVector());

	if ((dist < limitDist && dir > 0.f) || bHitted)
	{
		return true;
	}
	else
	{
		return false;
	}
}