// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Enemy.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

	void virtual BeginPlay() override;

	void virtual Tick(float DeltaSeconds) override;

	float limitDist;

	class AProjectRPGCharacter* Target;
	AEnemy* ControlledEnemy;

	void LookAtTarget();
	FVector FindDestination();

	void GetCombatDestiny(float _targetDist);
};
