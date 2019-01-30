// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "TargetingComponent.h"
#include "ProjectRPGCharacter.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	TargetingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Targeting_Sphere"));
	TargetingSphere->SetSphereRadius(1000.f);

	TargetIndex = 0;
	bLockOn = false;
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	MyOwner = Cast<AProjectRPGCharacter>(GetOwner());
	
}


// Called every frame
void UTargetingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	TArray<AActor*> AllActors;
	TargetingSphere->GetOverlappingActors(AllActors, AEnemy::StaticClass());
	
	if (AllActors.Num() > 0)
	{
		for (int32 i = 0; i < AllActors.Num(); i++)
		{
			AEnemy* const enemy = Cast<AEnemy>(AllActors[i]);

			if (enemy)
			{
				if (!CollectedEnemies.Contains(enemy) && FVector::Dist(enemy->GetActorLocation(), MyOwner->GetActorLocation()) < 1000.f)
				{
					CollectedEnemies.Add(enemy);
				}
				else if (CollectedEnemies.Contains(enemy) && FVector::Dist(enemy->GetActorLocation(), MyOwner->GetActorLocation()) > 1000.f)
				{
					CollectedEnemies.Remove(enemy);

					if (TargetIndex >= CollectedEnemies.Num())
						TargetIndex = CollectedEnemies.Num() - 1;
				}
			}
		}
	}

	if (CollectedEnemies.Num() > 0)
	{
		if(CollectedEnemies[TargetIndex]->dir > 0.f)
			CurrentTarget = CollectedEnemies[TargetIndex];
		else
			CurrentTarget = NULL;

		ClearTargets();
		ArrangeTargetsLogic();
		LockOnLogic(bLockOn);
	}
	else
	{
		CurrentTarget = NULL;
	}

	if (AllActors.Num() <= 0)
	{
		MyOwner->bInCombat = false;
		bLockOn = false;
		TargetIndex = 0;
		CollectedEnemies.Empty();
	}	
	else
	{
		MyOwner->bInCombat = true;
	}

	if(!bLockOn)
	{
		TargetIndex = 0;
		MyOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void UTargetingComponent::IndexUp()
{
	if (CollectedEnemies.Num() > 0 && bLockOn)
	{
		TargetIndex++;

		if (TargetIndex >= CollectedEnemies.Num())
		{
			TargetIndex = 0;
		}
	}

	GLog->Log("Index = " + FString::FromInt(TargetIndex));
}

void UTargetingComponent::IndexDown()
{
	if (CollectedEnemies.Num() > 0 && bLockOn)
	{
		TargetIndex--;

		if (TargetIndex < 0)
		{
			TargetIndex = CollectedEnemies.Num() - 1;
		}
	}

	GLog->Log("Index = " + FString::FromInt(TargetIndex));
}

void UTargetingComponent::LockOnTarget()
{
	if(MyOwner->bInCombat)
		bLockOn = !bLockOn;
}

void UTargetingComponent::LockOnLogic(bool _bLockOn)
{
	if (CurrentTarget)
	{
		if(bLockOn)
		{
			FRotator ControllerRot = MyOwner->GetControlRotation();
			FQuat ControllerQuat = MyOwner->GetController()->TransformComponent->GetComponentQuat();
			FVector NewRot = CurrentTarget->GetActorLocation() - MyOwner->GetActorLocation();
			NewRot.Normalize();

			FQuat RotToTarget = NewRot.Rotation().Quaternion();
			ControllerQuat = FQuat::Slerp(ControllerQuat, RotToTarget, 5.f * GetWorld()->GetDeltaSeconds());
			MyOwner->GetController()->SetControlRotation(FRotator(ControllerRot.Pitch, ControllerQuat.Rotator().Yaw, ControllerRot.Roll));
		}

		if (MyOwner->CombatComp->bIsAttacking || bLockOn) 
		{
			if (MyOwner->DefenseComp->dodgeTimer == 0.f)
			{
				FRotator OwnerRot = MyOwner->GetActorRotation();
				FQuat OwnerQuat = MyOwner->GetActorQuat();
				FVector NewRot = CurrentTarget->GetActorLocation() - MyOwner->GetActorLocation();
				NewRot.Normalize();

				FQuat RotToTarget = NewRot.Rotation().Quaternion();
				OwnerQuat = FQuat::Slerp(OwnerQuat, RotToTarget, 5.f * GetWorld()->GetDeltaSeconds());
				MyOwner->GetRootComponent()->SetWorldRotation(FRotator(OwnerRot.Pitch, OwnerQuat.Rotator().Yaw, OwnerRot.Roll));
				MyOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
			}
		}
	}
}

void UTargetingComponent::ArrangeTargetsLogic()
{
	if(!bLockOn && !MyOwner->CombatComp->bCanStringAttack && !MyOwner->CombatComp->bRunnigToEnemy)
		CollectedEnemies.Sort([](const AEnemy& LHS, const AEnemy& RHS) {return LHS.dir > RHS.dir; });
}

void UTargetingComponent::ClearTargets()
{
	for (int32 i = 0; i < CollectedEnemies.Num(); i++)
	{
		if (CollectedEnemies[i] == NULL)
		{
			CollectedEnemies.Empty();
			CurrentTarget = NULL;
		}
			
	}
}