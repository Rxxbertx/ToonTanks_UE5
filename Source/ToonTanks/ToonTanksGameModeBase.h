// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* DeadActor);

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowTankStats();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool State);


	
	UFUNCTION(BlueprintCallable)
	FTimerHandle& GetTimer(){return PlayerEnabled;}
	

private:
	
	class ATankPawn* Tank;
	class ATankPlayerController* Controller;
	float StartDelay = 3.f;
	FTimerHandle PlayerEnabled;
	void HandleGameStart();
	void GetTowersCount(int32& Towers) const;
	int32 Towers = 0;

	
	
};
