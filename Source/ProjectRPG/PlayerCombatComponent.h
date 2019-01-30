// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PlayerCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UPlayerCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCombatComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	class AProjectRPGCharacter* MyOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UAnimMontage*> AttackAnims;

	bool bCanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanStringAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRunnigToEnemy;

	int32 stringAttackNum;
	class AMeleeWeapon* OwnerWeapon;

	void Attacking(float Value);
	void StopAttacking();
	float DealDamage();

	//Blueprintable Functions
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void StringingAttacks();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void ResetStringAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void MeleeAttackSwung();
		
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AttackRested();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AttackReset();
	
};
