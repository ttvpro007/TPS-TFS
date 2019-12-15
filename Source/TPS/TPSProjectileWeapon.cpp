// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProjectileWeapon.h"

ATPSProjectileWeapon::ATPSProjectileWeapon()
{
	myOwner = Cast<ATPSCharacter>(GetOwner());
	World = GetWorld();
}

// Called when the game starts or when spawned
void ATPSProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoCount = StartAmmoCount;
}

void ATPSProjectileWeapon::Fire()
{
	myOwner = Cast<ATPSCharacter>(GetOwner());
	if (ProjectileClass)
	{
		if (myOwner)
		{
			if (CurrentAmmoCount > 0)
			{
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

				if (World)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;

					ProjectileSpawnPointLocation = meshComp->GetSocketLocation(muzzleSocketName);
					ProjectileSpawnPointRotation = meshComp->GetSocketRotation(muzzleSocketName);

					ATPSProjectile* const Projectile = World->SpawnActor<ATPSProjectile>
						(
							ProjectileClass,
							ProjectileSpawnPointLocation,
							ProjectileSpawnPointRotation,
							SpawnParams
							);

					if (Projectile)
					{
						FVector Start = myOwner->FireStartPos();
						FVector End = Start + myOwner->FireForwardDirection();
						FVector Direction = End - Start;

						Projectile->InitiateVelocity(Direction.GetSafeNormal());
					}
				}

				CurrentAmmoCount--;
			}
		}
		
	}
}