// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class AProjectile;
class UCapsuleComponent;

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:

	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* TowerMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Components")
	UCapsuleComponent* GetCapsuleComp() const { return CapsuleComp; }

	UFUNCTION(BlueprintCallable, Category = "Components")
	UStaticMeshComponent* GetBaseMesh() const { return BaseMesh; }

	UFUNCTION(BlueprintCallable, Category = "Components")
	UStaticMeshComponent* GetTowerMesh() const { return TowerMesh; }

	UFUNCTION(BlueprintCallable, Category = "Components")
	USceneComponent* GetProjectileSpawnPoint() const { return ProjectileSpawnPoint; }

	UClass* GetProjectileClass() const;
	virtual void Fire();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly,Category="CombatVisuals")
	UParticleSystem* DeathParticle;

	UPROPERTY(EditDefaultsOnly,Category="CombatSound")
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly,Category=CombatVisuals)
	TSubclassOf<class UCameraShakeBase> DeathCameraShake;

	
protected:
	
	bool RotateTurret(const FVector& LookAtTarget);
	virtual void HandleDestruction();
	
	
};
