// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "PlayerCombatComponent.h"
#include "PlayerDefenseComponent.h"
#include "TargetingComponent.h"
#include "ProjectRPGCharacter.generated.h"

UCLASS(config=Game)
class AProjectRPGCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AProjectRPGCharacter();

	// APawn interface
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool bCanMove;
	bool bCanRun;

	float launchForce;
	float zoomFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UPlayerCombatComponent* CombatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	class UPlayerDefenseComponent* DefenseComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	class UTargetingComponent* TargetComp;

	//Attack Mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		uint8 strenght;
	float baseAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		uint8 defense;
	float baseDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UClass* BPMeleeWeapon;

	AActor* MeleeWeapon;

	void OnAttackMode(float Value);
	void ExitAttackMode();

	void OnDefenseMode(float Value);
	void ExitDefenseMode();

	//Blueprint Functions
	UFUNCTION(BlueprintCallable, Category = "General")
		void Launcher();

	void TargetIndexUp();
	void TargetIndexDown();
	void LockOnEnemy();

	FVector LookVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float HitPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float MaxSP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float StaminaPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bCanSPRegain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float spCost;
	float SetSPCost();

	bool bLevelUp;
	bool bHitted;
	float curXp;
	float xpBoost;

	float takenDanage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 totalXp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 xpToLvl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		uint8 levelNum;

	void XpManager();

	UFUNCTION(BlueprintCallable, Category = "General")
		void StaminaExpended();
	
};

