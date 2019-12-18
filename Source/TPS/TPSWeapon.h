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
#include "Sound/SoundCue.h"
#include "TPSWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	SINGLE,
	AUTO,
	BURST
};

UENUM(BlueprintType)
enum class EWeaponZoomMode : uint8
{
	NORMAL,
	ZOOMED
};

class USoundCue;
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

	// fire mode
	bool bIsFiring;
	FTimerHandle FireTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	TEnumAsByte<EWeaponFireMode> WeaponFireMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float FireRatePerSec = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float BurstRatePerRound = 3;
	float BurstCount = 0;
	float FireInterval = 0;
	float FireTimer;
	UFUNCTION()
	void FireTimeCountingDown(float DeltaTime);

	// accuracy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float NormalSpreadAngle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float ZoomedSpreadAngle;
public:
	// accuracy
	UPROPERTY(BlueprintReadWrite, Category = "Weapon Properties")
	TEnumAsByte<EWeaponZoomMode> WeaponZoomMode;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon Properties")
	float SpreadAngle = NormalSpreadAngle;
	UFUNCTION(BlueprintCallable)
	void SetSpreadAngle(EWeaponZoomMode ZoomMode);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float SpreadAngleModifierValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float MaxSpreadAngle;

	// general properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	int MaxAmmoCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	int StartAmmoCount;
	UPROPERTY(BlueprintReadWrite, Category = "Weapon Properties")
	int CurrentAmmoCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float BaseDamage = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float DamageMultiplier = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float Range = 10000;

public:
	// zoom
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties", meta = (ClampMin = 0.1, ClampMax = 100))
	float zoomFOV;

protected:
	// vfx
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

	// sfx
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	USoundCue* GunShotEffect;

public:
	// fire
	UFUNCTION(BlueprintCallable)
	virtual void Fire();
	UFUNCTION(BlueprintCallable)
	void StartFire();
	UFUNCTION(BlueprintCallable)
	void EndFire();
	UFUNCTION(BlueprintCallable)
	void Reload();

	// bullet spread
	UFUNCTION(BlueprintCallable)
	FVector GetFireDirection();
	UFUNCTION(BlueprintCallable)
	FVector GetAimEndPoint();

	// accuracy
	UFUNCTION(BlueprintCallable)
	void IncreaseSpreadAngle(float IncrementValue, float MaxSpreadAngleValue);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};