// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"

// Sets default values
ATPSWeapon::ATPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Camera Comp"));
	RootComponent = meshComp;
}

// Called when the game starts or when spawned
void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	SpreadAngle = NormalSpreadAngle;
	CurrentAmmoCount = StartAmmoCount;
	FireInterval = 1 / FireRatePerSec;
	FireTimer = 0;
}

void ATPSWeapon::IncreaseSpreadAngle(float IncrementValue, float MaxSpreadAngleValue)
{
	SpreadAngle += IncrementValue;
	SpreadAngle = FMath::Min(SpreadAngle, MaxSpreadAngleValue);
}

// Called every frame
void ATPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTimeCountingDown(DeltaTime);
}

void ATPSWeapon::Fire()
{
	myOwner = Cast<ATPSCharacter>(GetOwner());

	if (!myOwner) return;

	if (CurrentAmmoCount > 0)
	{
		bIsFiring = true;

		FVector Start = myOwner->FireStartPos();
		FVector End = Start + GetFireDirection() * Range;

		if (muzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(
				muzzleEffect,
				Cast<USceneComponent>(meshComp),
				muzzleSocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator);
		}

		if (GunShotEffect)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, GunShotEffect, GetActorLocation());
		}

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(myOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
		{
			EPhysicalSurface SurfaceType =
				UPhysicalMaterial::DetermineSurfaceType(
					Cast<UPhysicalMaterial>(Hit.PhysMaterial));
			float mult = 1;
			switch (SurfaceType)
			{
			case FleshDefault:
				if (impactEffectFleshDefault)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectFleshDefault, Hit.Location);
				}
				break;
			case FleshVulnerable:
				if (impactEffectFleshVulnerable)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectFleshVulnerable, Hit.Location);
				}
				mult = DamageMultiplier;
				break;
			case Concrete:
				if (impactEffectConcrete)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectConcrete, Hit.Location);
				}
				break;
			default:
				if (impactEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffect, Hit.Location);
				}
				break;
			}

			UGameplayStatics::ApplyPointDamage(Hit.GetActor(), BaseDamage * mult, End - Start, Hit, GetInstigatorController(), this, damageClass);

			DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1);
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1);
		}

		CurrentAmmoCount--;
	}
	else bIsFiring = false; // set is firing to false if current bullet count is 0

	if (bIsFiring)
	{
		IncreaseSpreadAngle(SpreadAngleModifierValue, MaxSpreadAngle);
	}
}

void ATPSWeapon::FireTimeCountingDown(float DeltaTime)
{
	FireTimer -= DeltaTime;
	FireTimer = FMath::Max(FireTimer, 0.0f);
}

void ATPSWeapon::SetSpreadAngle(EWeaponZoomMode ZoomMode)
{
	switch (ZoomMode)
	{
	case EWeaponZoomMode::NORMAL:
		SpreadAngle = NormalSpreadAngle;
		break;
	case EWeaponZoomMode::ZOOMED:
		SpreadAngle = ZoomedSpreadAngle;
		break;
	default:
		break;
	}
}

void ATPSWeapon::StartFire()
{
	/*FireInterval = 1 / FireRatePerSec;
	FireTimer = FireInterval;*/

	switch (WeaponFireMode)
	{
	case EWeaponFireMode::SINGLE:
		if (FireTimer == 0)
		{
			Fire();
			FireTimer = FireInterval;
		}
		break;
	case EWeaponFireMode::AUTO:
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATPSWeapon::Fire, FireInterval, true, 0.0f);
		break;
	case EWeaponFireMode::BURST:
		if (BurstCount < BurstRatePerRound)
		{
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATPSWeapon::Fire, FireInterval, true, 0.0f);
			BurstCount++;
		}
		break;
	default:
		break;
	}
}

void ATPSWeapon::EndFire()
{
	bIsFiring = false;

	switch (WeaponFireMode)
	{
	case EWeaponFireMode::SINGLE:
		break;
	case EWeaponFireMode::AUTO:
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		break;
	case EWeaponFireMode::BURST:
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		BurstCount = 0;
		break;
	default:
		break;
	}
}

void ATPSWeapon::Reload()
{
	CurrentAmmoCount = MaxAmmoCount;
}

FVector ATPSWeapon::GetFireDirection()
{
	FVector RandomDirectionInCone = FVector::ZeroVector;

	if (myOwner)
	{
		FVector Start = myOwner->FireStartPos();
		FVector End = Start + myOwner->FireForwardDirection();

		float ConeHalfAngleRad = SpreadAngle / 2;

		RandomDirectionInCone = FMath::VRandCone(End - Start, ConeHalfAngleRad);
	}

	return RandomDirectionInCone;
}

FVector ATPSWeapon::GetAimEndPoint()
{
	FVector End = FVector::ZeroVector;

	if (myOwner)
	{
		FVector Start = meshComp->GetSocketLocation(muzzleSocketName);
		End = Start + myOwner->FireForwardDirection() * Range;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(myOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
		{
			return Hit.Location;
		}
	}

	return End;
}