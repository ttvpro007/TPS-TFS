// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackerBot.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

// Sets default values
ATrackerBot::ATrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = Cast<USceneComponent>(MeshComp);
	MeshComp->SetSimulatePhysics(true);

	// Sphere Trigger properties
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(destructionTriggerRadius);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrackerBot::StartSelfDamage);

	// Health Component properties
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ATrackerBot::HandleOnHealthChanged);
}

// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();
	NextPathPoint = CalculateNextPathPoint();
}

FVector ATrackerBot::CalculateNextPathPoint()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(
		this, GetActorLocation(), Cast<AActor>(player));
	if (path && path->PathPoints.Num() > 1)
	{
		return path->PathPoints[1];
	}
	return GetActorLocation();
}

void ATrackerBot::HandleOnHealthChanged(UHealthComponent* HealthCompVar, float Health, float DeltaHealth, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UGameplayStatics::SpawnSoundAtLocation(this, damageSoundEffect, GetActorLocation());
	if (!MatInst)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("DamageTime", UGameplayStatics::GetRealTimeSeconds(this));
	}
	if (Health <= 0)
	{
		SelfDestruct();
	}
}

void ATrackerBot::SelfDestruct()
{
	if(bExploded)
	{
		return;
	}
	bExploded = true;
	UGameplayStatics::SpawnEmitterAtLocation(this, explodeEffect, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, explodeSoundEffect, GetActorLocation());
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplodeRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplodeRadius, 24, FColor::Red, false, 5, 0, 3);
	Destroy();
}

void ATrackerBot::StartSelfDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorldTimerManager().SetTimer(selfDamageTimer, this, &ATrackerBot::DamageSelf, selfDamageTimeInterval, true, 0.0f);
}

void ATrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, selfDamage, GetInstigatorController(), this, nullptr);
}

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (player)
	{
		if (FVector::Dist(GetActorLocation(), player->GetActorLocation()) > closingDistance)
		{
			FVector force = NextPathPoint - GetActorLocation();
			force.Normalize();
			force *= MoveForce;
			MeshComp->AddForce(force, NAME_None, bUseVelocityChange);
		}
		if (FVector::Dist(GetActorLocation(), NextPathPoint) < closingDistance)
		{
			NextPathPoint = CalculateNextPathPoint();
		}
	}
}

// Called to bind functionality to input
void ATrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

