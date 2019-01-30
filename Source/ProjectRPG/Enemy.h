// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyCombatComponent.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECTRPG_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float HealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float strenght;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float xp;

	bool bHitted;
	class AProjectRPGCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	UEnemyCombatComponent* CombatComp;

	float DirectionToPlayer();
	bool PlayerInSightAndDist(float limitDist);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float sightDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bLockedOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bStaggered;

	float stagTimer;
	float stagCoolDown;
};
