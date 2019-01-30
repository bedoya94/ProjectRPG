// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "EnemyCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UEnemyCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyCombatComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	class AProjectRPGCharacter* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* HitBox;

	class AEnemy* Owner;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float attackTimer;
	float attackCoolDown;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool bCanAttack;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool bAttack;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool bAttacking;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<UAnimMontage*> AttackMontages;

	TArray<AActor*> ThingsHit;
	void OnAttacking();
	float DealDamage();

	UFUNCTION(BlueprintCallable, Category = "EnemyCombat")
	void ResetAttack();

	//Collision
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void HitCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};
