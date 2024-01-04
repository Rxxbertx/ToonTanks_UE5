// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "TankPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TowerPawn.h"
#include "Particles/ParticleSystemComponent.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	/// Create components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SmokeTrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	SmokeTrailParticle->SetupAttachment(BaseMesh);
	SmokeTrailParticle1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System 1"));
	SmokeTrailParticle1->SetupAttachment(BaseMesh);
}


void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the turret is currently in the process of rotating
	if (RotatingTurret)
	{
		// Call RotateTurret function with the impact point obtained from the last hit result
		// The RotateTurret function returns a boolean indicating whether the rotation is complete
		RotatingTurret = RotateTurret(HitResult.ImpactPoint);
	}
}


void ATankPawn::MoveCompleted()
{
	SmokeTrailParticle->Deactivate();
	SmokeTrailParticle1->Deactivate();
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATankPawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATankPawn::MoveCompleted);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATankPawn::Fire);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATankPawn::Rotate);
	}
}

void ATankPawn::CheckFireCondition()
{
	CanFire = true;
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	MoveCompleted();
	APlayerControllerPointer = Cast<APlayerController>(GetController());

	const FVector TowardsGroundLocation = GetActorLocation() + (GetActorUpVector() * -100.f);
	SetActorLocation(TowardsGroundLocation, true);
	BaseMesh->SetWorldLocation(TowardsGroundLocation, true);


	if (APlayerControllerPointer)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(APlayerControllerPointer->GetLocalPlayer()))

		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void ATankPawn::Move(const FInputActionValue& value)
{
	const FVector2d MoveVector = value.Get<FVector2d>();

	if (GetController())
	{
		if (!SmokeTrailParticle->IsActive() && !SmokeTrailParticle1->IsActive())
		{
			SmokeTrailParticle->Activate();
			SmokeTrailParticle1->Activate();
		}


		// Calcula el vector de movimiento en función de la dirección del control
		const FVector MoveDirection = FVector(MoveVector.Y, 0, 0);
		const FRotator RotateDirection = FRotator(0, MoveVector.X, 0);

		// Calcula el desplazamiento basado en la velocidad actual
		const FVector MoveOffset = MoveDirection * GetWorld()->GetDeltaSeconds() * MoveSpeed;
		const FRotator RotationOffset = RotateDirection * GetWorld()->GetDeltaSeconds() * RotationSpeed;


		// Aplica el desplazamiento local al actor
		AddActorLocalOffset(MoveOffset, true);
		AddActorLocalRotation(RotationOffset, true);
	}
}


void ATankPawn::Rotate(const FInputActionValue& Value)
{
	if (APlayerControllerPointer && APlayerControllerPointer->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		RotatingTurret = true;
	}
}


void ATankPawn::Fire()
{
	if (CanFire)
	{
		CanFire = false;
		Super::Fire();
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATankPawn::CheckFireCondition, FireRate);
	}
}

void ATankPawn::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}
