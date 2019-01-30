// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PlayerDefenseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UPlayerDefenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerDefenseComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	class AProjectRPGCharacter* MyOwner;

	void OnDefending(float Value);
	void StopDefending();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		bool bDefending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		bool bIsDodging;

	UPROPERTY(EditAnywhere, Category = "Defense")
		TArray<UAnimMontage*> DefenseAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		bool bCanDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float dodgeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		bool bCountering;
		bool bCanCounter;

	float counterTimer;
	float counterLimit;

	float dodgeCoolDown;
	void RollDodge();
	
};
