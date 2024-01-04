// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"

#include "TankPawn.h"

ATowerPawn::ATowerPawn()
{
}


void ATowerPawn::CheckFireCondition()
{
	if (TankPawn && HasEnoughRange() && TankPawn->GetAlive())
	{
		Fire();
	}

	
}

void ATowerPawn::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATowerPawn::CheckFireCondition, FireRate, true, 0.f);
	
}

void ATowerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPawn && HasEnoughRange())
	{
		RotateTurret(TankPawn->GetActorLocation());
	}
	
}

bool ATowerPawn::HasEnoughRange() const
{
	const float Distance = FVector::Dist(GetActorLocation(), TankPawn->GetActorLocation());
	return Distance <= FireRange;
}


void ATowerPawn::Fire()
{
	Super::Fire();
}

void ATowerPawn::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
	
}
