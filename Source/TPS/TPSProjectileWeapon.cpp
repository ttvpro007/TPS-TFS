// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProjectileWeapon.h"

void ATPSProjectileWeapon::Fire()
{
	if (ProjectileClass)
	{
		if (BulletCount > 0)
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

			UWorld* const World = GetWorld();
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
					Projectile->InitiateVelocity(ProjectileSpawnPointRotation.Vector());
				}
			}

			BulletCount--;
		}
		
	}
}