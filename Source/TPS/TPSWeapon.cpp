// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TPS.h"

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
	
}

// Called every frame
void ATPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSWeapon::Fire()
{
	ATPSCharacter* myOwner = Cast<ATPSCharacter>(GetOwner());
	if (myOwner)
	{
		FVector start = myOwner->FireStartPos();
		FVector end = start + myOwner->FireForwardDirection() * Range;

		if (muzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(
				muzzleEffect, 
				Cast<USceneComponent>(meshComp), 
				muzzleSocketName, 
				FVector::ZeroVector, 
				FRotator::ZeroRotator);
		}

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(myOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, QueryParams))
		{
			EPhysicalSurface SurfaceType = 
				UPhysicalMaterial::DetermineSurfaceType(
					Cast<UPhysicalMaterial>(hit.PhysMaterial));
			float mult = 1;
			switch (SurfaceType)
			{
			case FleshDefault:
				if (impactEffectFleshDefault)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectFleshDefault, hit.Location);
				}
				break;
			case FleshVulnerable:
				if (impactEffectFleshVulnerable)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectFleshVulnerable, hit.Location);
				}
				mult = DamageMultiplier;
				break;
			case Concrete:
				if (impactEffectConcrete)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectConcrete, hit.Location);
				}
				break;
			default:
				if (impactEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffect, hit.Location);
				}
				break;
			}

			UGameplayStatics::ApplyPointDamage(hit.GetActor(), BaseDamage * mult, end - start, hit, GetInstigatorController(), this, damageClass);

			DrawDebugLine(GetWorld(), start, hit.Location, FColor::Red, false, 1);
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 1);
		}
	}
}

