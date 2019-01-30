// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRPG.h"
#include "RPGPlayerController.h"
#include "ProjectRPGCharacter.h"

ARPGPlayerController::ARPGPlayerController()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	AProjectRPGCharacter* const RPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

	InputComponent->BindAction("Run", IE_Pressed, this, &ARPGPlayerController::Running);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARPGPlayerController::Jumping);
	InputComponent->BindAction("Jump", IE_Released, this, &ARPGPlayerController::StopJump);
	InputComponent->BindAction("RollDodge", IE_Pressed, this, &ARPGPlayerController::Rolling);

	InputComponent->BindAxis("MoveForward", this, &ARPGPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARPGPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	
	InputComponent->BindAxis("TurnRate", this, &ARPGPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &ARPGPlayerController::LookUpAtRate);

}

void ARPGPlayerController::TurnAtRate(float Rate)
{
	AProjectRPGCharacter* const RPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());
	// calculate delta for this frame from the rate information
	if(RPGPlayer && !RPGPlayer->TargetComp->bLockOn)
		RPGPlayer->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerController::LookUpAtRate(float Rate)
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());
	// calculate delta for this frame from the rate information
	if (MyRPGPlayer)
		MyRPGPlayer->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerController::MoveForward(float Value)
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

	
		if ((MyRPGPlayer) && (MyRPGPlayer->Controller != NULL) && (Value != 0.f) && MyRPGPlayer->bCanMove)
		{
			// find out which way is forward
			const FRotator Rotation = MyRPGPlayer->Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			MyRPGPlayer->AddMovementInput(Direction, Value);
		}
}

void ARPGPlayerController::MoveRight(float Value)
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

		if ((MyRPGPlayer) && (MyRPGPlayer->Controller != NULL) && (Value != 0.f) && MyRPGPlayer->bCanMove)
		{
			// find out which way is right
			const FRotator Rotation = MyRPGPlayer->Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			MyRPGPlayer->AddMovementInput(Direction, Value);
		}
}

void ARPGPlayerController::Running()
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

	if (!MyRPGPlayer->GetCharacterMovement()->Velocity.IsZero() && !MyRPGPlayer->DefenseComp->bDefending)
		MyRPGPlayer->bCanRun = true;
}

void ARPGPlayerController::Jumping()
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

	if (MyRPGPlayer)
	{
		if (MyRPGPlayer->bCanMove && !MyRPGPlayer->DefenseComp->bDefending && !MyRPGPlayer->DefenseComp->bCanDodge)
		{
			MyRPGPlayer->Jump();
		}
		else if(MyRPGPlayer->DefenseComp->bDefending && MyRPGPlayer->DefenseComp->bCanDodge)
		{
			MyRPGPlayer->DefenseComp->RollDodge();
		}
		
	}
}

void ARPGPlayerController::StopJump()
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

	if (MyRPGPlayer && !MyRPGPlayer->DefenseComp->bDefending)
	{
		MyRPGPlayer->StopJumping();
	}
}

void ARPGPlayerController::Rolling()
{
	AProjectRPGCharacter* const MyRPGPlayer = Cast<AProjectRPGCharacter>(GetPawn());

	if (MyRPGPlayer->DefenseComp->bCanDodge)
	{
		MyRPGPlayer->DefenseComp->RollDodge();
	}
}