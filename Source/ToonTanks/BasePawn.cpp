// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/// Create components
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));

	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	TowerMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TowerMesh);
}


bool ABasePawn::RotateTurret(const FVector& LookAtTarget)
{
	// Get the current rotation of the TowerMesh
	const FRotator TowerMeshRotation = TowerMesh->GetComponentRotation();

	// Calculate the distance between the current location and the target location
	const FVector Distance = LookAtTarget - TowerMesh->GetComponentLocation();

	// Calculate the rotation needed to face the LookAtTarget
	const FRotator Rotation(0, Distance.Rotation().Yaw, 0);

	// Check if the current turret rotation is approximately equal to the desired rotation
	if (TowerMeshRotation.Equals(FRotator(TowerMeshRotation.Pitch, Distance.Rotation().Yaw, TowerMeshRotation.Roll),
	                             0.5))
	{
		// If already facing the target, return false
		return false;
	}

	// Interpolate the turret rotation using FMath::RInterpTo for smooth rotation
	const FRotator InterpRotation = FMath::RInterpTo(TowerMesh->GetComponentRotation(), Rotation,
	                                                 GetWorld()->GetDeltaSeconds(), 10);

	// Set the world rotation of the TowerMesh to the interpolated rotation
	TowerMesh->SetWorldRotation(InterpRotation);

	// Draw a debug sphere at the LookAtTarget position
	DrawDebugSphere(GetWorld(), LookAtTarget, 20, 10, FColor::Red, false, -1);

	// Return true to indicate that the turret has been rotated
	return true;
}

void ABasePawn::HandleDestruction()
{
	//Visual, sound effects etc

	if (DeathCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShake);
	}

	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

UClass* ABasePawn::GetProjectileClass() const
{
	return ProjectileClass.Get();
}

void ABasePawn::Fire()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
	                                    ProjectileSpawnPoint->GetComponentRotation(), SpawnParams);
}
