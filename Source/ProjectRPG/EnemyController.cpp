// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "ProjectRPGCharacter.h"
#include "EnemyController.h"


AEnemyController::AEnemyController()
{
	limitDist = 500.f;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	ControlledEnemy = Cast<AEnemy>(GetPawn());
}

void AEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Target = Cast<AProjectRPGCharacter>(ControlledEnemy->Player);

	if (Target != nullptr)
	{
		float distToTarget = FVector::Dist(Target->GetActorLocation(), ControlledEnemy->GetActorLocation());

		if (ControlledEnemy->PlayerInSightAndDist(700.f))
		{
			LookAtTarget();

			if (ControlledEnemy->CombatComp->bCanAttack)
				limitDist = 150.f;
			else
				limitDist = 300.f;

			if (!ControlledEnemy->bStaggered)
			{
				if (distToTarget > limitDist)
					MoveToLocation(FindDestination(), limitDist, true, false);
				else if (distToTarget < limitDist || ControlledEnemy->CombatComp->bAttacking)
					StopMovement();
			}
		}
	}
}

void AEnemyController::LookAtTarget()
{
	FVector NewRot = ControlledEnemy->Player->GetActorLocation() - ControlledEnemy->GetActorLocation();
	NewRot.Normalize();
	FRotator RotToTarget = NewRot.Rotation();
	RotToTarget.Pitch = 0.f;

	ControlledEnemy->GetRootComponent()->SetWorldRotation(RotToTarget);
}

FVector AEnemyController::FindDestination()
{
	FVector targetDesPos = FVector::ZeroVector;

	targetDesPos = Target->GetActorLocation();

	return targetDesPos;
}

void AEnemyController::GetCombatDestiny(float _targetDist)
{
	if (_targetDist > limitDist)
		MoveToLocation(FindDestination(), limitDist, true, false);
	else if (_targetDist < limitDist || ControlledEnemy->CombatComp->bAttacking)
		StopMovement();
}