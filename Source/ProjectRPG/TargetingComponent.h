// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Enemy.h"
#include "TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	class AProjectRPGCharacter* MyOwner;

	USphereComponent* TargetingSphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		AEnemy* CurrentTarget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<AEnemy*> CollectedEnemies;

	int32 TargetIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bLockOn;

	void IndexUp();
	void IndexDown();
	void LockOnTarget();

	void LockOnLogic(bool _bLockOn);
	void ArrangeTargetsLogic();
	void ClearTargets();
};
