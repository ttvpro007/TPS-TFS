// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Math/UnrealMathUtility.h"
#include "TPSWeapon.generated.h"

UENUM(BlueprintType)
enum EWeaponFireMode
{
	SINGLE UMETA(DysplayName = "Single Fire"),
	AUTO UMETA(DysplayName = "Automatic Fire"),
	BURST UMETA(DysplayName = "Burst Fire")
};

class UParticleSystem;
class USkeletalMeshComponent;
UCLASS()
class TPS_API ATPSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ATPSCharacter* myOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	USkeletalMeshComponent* meshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	TSubclassOf<UDamageType> damageClass;

	bool bIsFiring;
	FTimerHandle FireTimerHandle;
	FTimerHandle AccuracyTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	TEnumAsByte<EWeaponFireMode> WeaponFireMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float FireRatePerSec = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float BurstRatePerRound = 3;
	float BurstCount = 0;
	float FireInterval = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float MinAccuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float MaxAccuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float Accuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float AccuracyModifier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float AccuracyModifyInterval = 0.2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float MinZoomAccuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float MaxZoomAccuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float ZoomAccuracy;
	float IntervalTimer = 0.2 + 1;

	void DecreaseAccuracy();
	void IncreaseAccuracy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	int MaxBulletCount = 100;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Properties")
	int BulletCount = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float BaseDamage = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float DamageMultiplier = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float Range = 10000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	UParticleSystem* muzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	UParticleSystem* impactEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	UParticleSystem* impactEffectFleshDefault;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	UParticleSystem* impactEffectFleshVulnerable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	UParticleSystem* impactEffectConcrete;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	FName muzzleSocketName;
public:
	UFUNCTION(BlueprintCallable)
	virtual void Fire();

public:
	UFUNCTION(BlueprintCallable)
	void StartFire();
	UFUNCTION(BlueprintCallable)
	void EndFire();
	UFUNCTION(BlueprintCallable)
	void Reload();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties", meta = (ClampMin = 0.1, ClampMax = 100))
	float zoomFOV;
	UFUNCTION(BlueprintCallable)
	FVector GetFireDirection();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};