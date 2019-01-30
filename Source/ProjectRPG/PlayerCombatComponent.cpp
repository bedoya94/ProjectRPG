// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "PlayerCombatComponent.h"
#include "ProjectRPGCharacter.h"
#include "MeleeWeapon.h"


// Sets default values for this component's properties
UPlayerCombatComponent::UPlayerCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	bCanAttack = false;
	bIsAttacking = false;
	stringAttackNum = 0;
	bHeavyAttack = false;
}


// Called when the game starts
void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = Cast<AProjectRPGCharacter>(GetOwner());

	if (MyOwner->MeleeWeapon)
	{
		OwnerWeapon = Cast<AMeleeWeapon>(MyOwner->MeleeWeapon);
	}
}


// Called every frame
void UPlayerCombatComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!bHeavyAttack)
	{
		if (stringAttackNum > 0 && stringAttackNum < 4.f)
		{
			MyOwner->launchForce = 300.f;
		}
	}
	else
	{
		stringAttackNum = 5;
		MyOwner->launchForce = 700.f;
	}

	if (MyOwner->TargetComp->CurrentTarget)
	{
		FVector TargetLocation = MyOwner->TargetComp->CurrentTarget->GetActorLocation();

		if ((FVector::Dist(TargetLocation, MyOwner->GetActorLocation()) < 150.f))
		{
			bRunnigToEnemy = false;
		}
		else if((FVector::Dist(TargetLocation, MyOwner->GetActorLocation()) > 150.f) && bCanAttack)
		{
			bRunnigToEnemy = true;
		}

		if (bRunnigToEnemy)
		{
			MyOwner->DefenseComp->bCountering = false;
			bIsAttacking = false;
			bCanStringAttack = false;
			bHeavyAttack = false;
			stringAttackNum = 0;
			MyOwner->bCanRun = true;

			FVector toEnemy = TargetLocation - MyOwner->GetActorLocation();
			float distToEnemy = toEnemy.Size();

			toEnemy /= distToEnemy;

			MyOwner->AddMovementInput(toEnemy, 700.f * DeltaTime);
		}
	}	
}

void UPlayerCombatComponent::Attacking(float Value)
{
	if (!MyOwner->DefenseComp->bDefending && MyOwner->DefenseComp->dodgeTimer == 0.f)
	{
		if (Value > 0.f && MyOwner->StaminaPoints >= MyOwner->spCost)
		{
			bCanAttack = true;

			if (!bRunnigToEnemy)
			{
				bIsAttacking = true;
				bCanStringAttack = true;
				MyOwner->bCanMove = false;

				if (stringAttackNum == 0)
				{
					MyOwner->PlayAnimMontage(AttackAnims[0]);
					stringAttackNum = 2;
				}

				if (bHeavyAttack)
				{
					MyOwner->PlayAnimMontage(AttackAnims[4]);
					bHeavyAttack = false;
				}
			}
		}
	}
}

void UPlayerCombatComponent::StopAttacking()
{
	bCanAttack = false;
	bIsAttacking = false;
}

void UPlayerCombatComponent::StringingAttacks()
{
	if (bCanStringAttack)
	{
		switch (stringAttackNum)
		{
			case 1:
				MyOwner->PlayAnimMontage(AttackAnims[0], 1.f, "Begin");
				stringAttackNum = 2;
				break;
			case 2:
				MyOwner->PlayAnimMontage(AttackAnims[1], 1.f, "AttackStart");
				stringAttackNum = 3;
				break;
			case 3:
				MyOwner->PlayAnimMontage(AttackAnims[2], 1.f, "AttackStart");
				stringAttackNum = 4;
				break;
			case 4:
				MyOwner->PlayAnimMontage(AttackAnims[3], 1.f, "AttackStart");
				stringAttackNum = 1;
				break;
		}
	}
	
}

void UPlayerCombatComponent::ResetStringAttack()
{
	bCanStringAttack = false;
}

void UPlayerCombatComponent::AttackReset()
{
	MyOwner->bCanSPRegain = true;
	MyOwner->bCanMove = true;
	MyOwner->DefenseComp->bCountering = false;
	bRunnigToEnemy = false;
	bCanAttack = false;
	bIsAttacking = false;
	bCanStringAttack = false;
	bHeavyAttack = false;
	stringAttackNum = 0;

	if(OwnerWeapon)
		OwnerWeapon->bSwinging = false;
}

void UPlayerCombatComponent::MeleeAttackSwung()
{
	if (OwnerWeapon)
	{
		OwnerWeapon->Swing();
	}
}

void UPlayerCombatComponent::AttackRested()
{
	if (OwnerWeapon)
	{
		OwnerWeapon->Rest();
	}
}

float UPlayerCombatComponent::DealDamage()
{
	float ownerDamage = MyOwner->strenght;
	float ownerWeaponDamage = 0.f;

	AMeleeWeapon* const OwnerWeapon = Cast<AMeleeWeapon>(MyOwner->MeleeWeapon);

	if (OwnerWeapon)
	{
		ownerWeaponDamage = OwnerWeapon->AttackDamage;
	}

	float totalDamage = ownerDamage + ownerWeaponDamage;

	if (stringAttackNum < 5)
	{
		totalDamage *= 1.f;
	}
	else
	{
		totalDamage *= 2.f;
	}

	return totalDamage;
}