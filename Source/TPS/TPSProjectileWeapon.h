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
	virtual void Fire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	TSubclassOf<class ATPSProjectile> ProjectileClass;

private:
	FVector ProjectileSpawnPointLocation;
	FRotator ProjectileSpawnPointRotation;
};
