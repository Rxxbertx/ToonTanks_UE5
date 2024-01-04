// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameModeBase.h"

#include "TankPawn.h"
#include "TankPlayerController.h"
#include "TowerPawn.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameModeBase::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();

		if (Controller)
		{
			Controller->SetPlayerEnabledState(false);
		}

		GameOver(true);
	}
	else if (ATowerPawn* DestroyedTowerPawn = Cast<ATowerPawn>(DeadActor))
	{
		DestroyedTowerPawn->HandleDestruction();

		if (Towers)
		{
			--Towers;
			if (Towers <= 0)
			{
				if (Controller)
				{
					Controller->SetPlayerEnabledState(false);
				}
				GameOver(false);
			}
		}
	}
}

void AToonTanksGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
	GetTowersCount(Towers);
}

void AToonTanksGameModeBase::HandleGameStart()
{
	Tank = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Tank)
	{
		Controller = Cast<ATankPlayerController>(Tank->GetController());
	}

	if (Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("CONTROLLED"))
		Controller->SetPlayerEnabledState(false);


		const FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
			Controller, &ATankPlayerController::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(PlayerEnabled, InputDelegate, StartDelay, false);
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AToonTanksGameModeBase::ShowTankStats, 1.f, false, StartDelay);
	}

	StartGame();
}


void AToonTanksGameModeBase::GetTowersCount(int32& OutTowers) const
{
	TArray<AActor*> TowersActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATowerPawn::StaticClass(), TowersActors);
	OutTowers = TowersActors.Num();
	UE_LOG(LogTemp, Display, TEXT("TOWERS COUNT %d"), OutTowers);
}
