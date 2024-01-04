// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Impulse,
	           const FHitResult& HitResult);

private:

	UPROPERTY(Category=Components, EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(Category=Combat,VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category=Combat)
	float Damage= 25.f;

	UPROPERTY(EditDefaultsOnly, Category=CombatVisuals)
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category=CombatVisuals)
	UParticleSystemComponent* SmokeTrailParticle;

	UPROPERTY(EditAnywhere, Category=CombatSound)
	USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere, Category=CombatSound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere,Category=CombatVisuals)
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

};
