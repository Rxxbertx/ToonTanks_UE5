// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "TankPawn.generated.h"


class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOONTANKS_API ATankPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	ATankPawn();
	virtual void HandleDestruction() override;


	UFUNCTION(BlueprintCallable, Category = "Components")
	UCameraComponent* GetCamera() const { return Camera; };

private:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category= Input)
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category= Input)
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, Category= Input)
	UInputAction* RotateAction;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputMappingContext* MappingContext;

	APlayerController* APlayerControllerPointer;
	float FireRate = 0.2f;
	bool CanFire = true;
	FTimerHandle FireRateTimerHandle;

public:
	UFUNCTION(BlueprintCallable)
	bool GetCanFire() const
	{
		return CanFire;
	}

	UFUNCTION(BlueprintCallable)
	FTimerHandle GetFireRateTimerHandle() const
	{
		return FireRateTimerHandle;
	}

	bool GetAlive() const { return bAlive; };

protected:
	// To add mapping context
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Move(const FInputActionValue& value);
	void Rotate(const FInputActionValue& value);
	virtual void Fire() override;
	void MoveCompleted();
	void CheckFireCondition();

	UPROPERTY(EditAnywhere, Category=Movement)
	float MoveSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category=Movement)
	float RotationSpeed = 100.f;

	FHitResult HitResult;
	bool RotatingTurret;
	bool bAlive = true;


	UPROPERTY(EditDefaultsOnly, Category=CombatVisuals)
	UParticleSystemComponent* SmokeTrailParticle;
	UPROPERTY(EditDefaultsOnly, Category=CombatVisuals)
	UParticleSystemComponent* SmokeTrailParticle1;
};
