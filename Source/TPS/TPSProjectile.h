// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TPSProjectile.generated.h"

class USoundCue;
class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class TPS_API ATPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	UProjectileMovementComponent* ProjectileMovement;

	bool bExploded = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	UParticleSystem* explodeEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	USoundCue* explodeSoundEffect;
	FTimerHandle ExplosionTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float ExplosionTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float ExplosionDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float ExplodeRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float InitialSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void InitiateVelocity(const FVector& ShootDirection);
	UFUNCTION(BlueprintCallable)
	virtual void Explode();

};
