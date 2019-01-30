// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectRPGCharacter.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class PROJECTRPG_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
		float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
		bool bSwinging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
		TArray<AActor*> ThingsHit;

	AProjectRPGCharacter* WeaponHolder;

	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
		UBoxComponent* HitBox;

	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

		void Swing();
		void Rest();

	void AttachWeaponToHolder();
	
};
