// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSWeapon.h"
#include "Engine/World.h"
#include "TPSProjectile.h"
#include "TPSProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSProjectileWeapon : public ATPSWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATPSProjectileWeapon();
	virtual void Fire() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	TSubclassOf<class ATPSProjectile> ProjectileClass;

	UWorld* World;

private:
	FVector ProjectileSpawnPointLocation;
	FRotator ProjectileSpawnPointRotation;
};
