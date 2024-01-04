// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "TowerPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATowerPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	ATowerPawn();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	class ATankPawn* TankPawn = nullptr;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 2.5f;

	FTimerHandle FireRateTimerHandle;
	
	bool HasEnoughRange() const;
	void CheckFireCondition();

	virtual void Fire() override;
public:
	
	virtual void HandleDestruction() override;
};
