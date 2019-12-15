// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProjectile.h"

// Sets default values
ATPSProjectile::ATPSProjectile(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// default values
	ExplosionTime = 3;
	ExplosionDamage = 80;
	ExplodeRadius = 500;
	InitialSpeed = 3000;

	// collision component
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereCollider"));
	CollisionComp->InitSphereRadius(0.2f);
	RootComponent = CollisionComp;

	// projectile component
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = 3000;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.1f;
}

// Called when the game starts or when spawned
void ATPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ATPSProjectile::Explode, ExplosionTime, true, 0.0f);
}

// Called every frame
void ATPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSProjectile::InitiateVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ATPSProjectile::Explode()
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;
	UGameplayStatics::SpawnEmitterAtLocation(this, explodeEffect, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, explodeSoundEffect, GetActorLocation());
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplodeRadius, nullptr, IgnoredActors, this, GetInstigatorController(), false, ECollisionChannel::ECC_Visibility);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplodeRadius, 24, FColor::Red, false, 5, 0, 3);
	Destroy();
}