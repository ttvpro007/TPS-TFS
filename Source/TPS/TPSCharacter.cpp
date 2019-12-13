// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPSWeapon.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ATPSCharacter::HandleOnHealthChanged);
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	currentWeapon = GetWorld()->SpawnActor<ATPSWeapon>(weaponType);
	currentWeapon->AttachToComponent(Cast<USceneComponent>(GetMesh()),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		"WeaponSocket");
	currentWeapon->SetOwner(this);
	currentSpeed = defaultSpeed;
}

void ATPSCharacter::HandleOnHealthChanged(UHealthComponent* HealthCompVar, float Health, float DeltaHealth, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0)
	{
		PlayerCharacterState = CharacterStateEnum::Dead;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, deathMaterial);
		GetMesh()->SetScalarParameterValueOnMaterials("StartTime", UGameplayStatics::GetRealTimeSeconds(this));
		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacterState == CharacterStateEnum::InCover && CoverTriggerVolume)
	{
		FRotator rot = CoverTriggerVolume->GetComponentRotation();
		rot = rot.Add(0, 50, 0);
		GetMesh()->SetWorldRotation(rot);
	}

	if (PlayerCharacterState == CharacterStateEnum::SlidingIntoCover)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetCoverPos, DeltaTime, slidingIntoCoverSpeed));
		if (CoverTriggerVolume)
		{
			FRotator rot = CoverTriggerVolume->GetComponentRotation();
			rot = rot.Add(0, 50, 0);
			GetMesh()->SetWorldRotation(FMath::RInterpTo(GetMesh()->GetComponentRotation(), rot, DeltaTime, slidingIntoCoverSpeed));

			if (FVector::Distance(GetActorLocation(), TargetCoverPos) < closingDistance)
			{
				PlayerCharacterState = CharacterStateEnum::InCover;
			}
		}
	}
}

void ATPSCharacter::MoveForward(float axis)
{
	if (PlayerCharacterState == CharacterStateEnum::Walking)
	{
		AddMovementInput(GetActorForwardVector() * axis, currentSpeed);
	}
}

void ATPSCharacter::MoveSideways(float axis)
{
	switch (PlayerCharacterState)
	{
	case Walking:
		AddMovementInput(GetActorRightVector() * axis, currentSpeed);
		break;
	case SlidingIntoCover:
		break;
	case InCover:
		if (CoverTriggerVolume)
		{
			FVector RelativeLoc = GetActorLocation() - CoverTriggerVolume->GetComponentLocation();
			float Dot = FVector::DotProduct(RelativeLoc, CoverTriggerVolume->GetRightVector());
			bool MovingInwards = Dot * axis < 0;
			bool withinBorders = abs(Dot) < CoverTriggerVolume->GetScaledBoxExtent().Y - CoverPadding;
			if (MovingInwards || withinBorders)
			{
				AddMovementInput(CoverTriggerVolume->GetRightVector() * axis, currentSpeed);
			}
		}
		break;
	case InCoverAndAiming:
		break;
	default:
		break;
	}
}

void ATPSCharacter::StartCrouch()
{
	Crouch();
}

void ATPSCharacter::EndCrouch()
{
	UnCrouch();
}

void ATPSCharacter::StartZoom()
{
	bZoomed = true;
	if (PlayerCharacterState == CharacterStateEnum::InCover)
	{
		PlayerCharacterState = CharacterStateEnum::InCoverAndAiming;
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
}

void ATPSCharacter::EndZoom()
{
	bZoomed = false;
	if (PlayerCharacterState == CharacterStateEnum::InCoverAndAiming)
	{
		PlayerCharacterState = CharacterStateEnum::InCover;
		FRotator rot = CoverTriggerVolume->GetComponentRotation();
		rot = rot.Add(0, 50, 0);
		GetMesh()->SetWorldRotation(rot);
	}
}

void ATPSCharacter::StartRun()
{
	currentSpeed = runSpeed;
	bRunning = true;
}

void ATPSCharacter::EndRun()
{
	currentSpeed = defaultSpeed;
	bRunning = false;
}

FVector ATPSCharacter::FireStartPos()
{
	return GetPawnViewLocation();
}

FVector ATPSCharacter::FireForwardDirection()
{
	return GetViewRotation().Vector();
}

void ATPSCharacter::Fire()
{
	currentWeapon->Fire();
}

void ATPSCharacter::TakeCover()
{
	switch (PlayerCharacterState)
	{
	case Walking:
		if (CoverTriggerVolume)
		{
			TakeCoverStartLocation = GetActorLocation();
			TakeCoverStartRotation = GetActorRotation();
			FVector TraceEnd = GetActorLocation() + CoverTriggerVolume->GetForwardVector() * 1000;
			FHitResult hit;
			FCollisionQueryParams traceParams;
			traceParams.AddIgnoredActor(this);
			DrawDebugLine(GetWorld(), TakeCoverStartLocation, TraceEnd, FColor::Red, false, 5, 0, 2);
			if (GetWorld()->LineTraceSingleByChannel(hit, TakeCoverStartLocation, TraceEnd, ECC_Visibility, traceParams))
			{
				TargetCoverPos = hit.Location - 
					GetCapsuleComponent()->GetScaledCapsuleRadius() * 
					CoverTriggerVolume->GetForwardVector();
				PlayerCharacterState = CharacterStateEnum::SlidingIntoCover;
			}
		}
		break;
	case SlidingIntoCover:
		break;
	case InCover:
		PlayerCharacterState = CharacterStateEnum::Walking;
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		break;
	case InCoverAndAiming:
		break;
	default:
		break;
	}
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

