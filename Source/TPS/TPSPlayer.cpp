// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPSWeapon.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShake.h"

ATPSPlayer::ATPSPlayer()
{
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	springComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Comp"));
	springComp->SetupAttachment(RootComponent);
	cameraComp->SetupAttachment(springComp);
	springComp->bUsePawnControlRotation = true;
}

FVector ATPSPlayer::FireStartPos()
{
	return cameraComp->GetComponentLocation();
}

FVector ATPSPlayer::FireForwardDirection()
{
	return cameraComp->GetForwardVector();
}

void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float targetFOV = 0;
	if (bZoomed)
	{
		targetFOV = currentWeapon->zoomFOV;
	}
	else
	{
		targetFOV = defaultFOV;
	}
	currentFOV = FMath::FInterpTo(currentFOV, targetFOV, DeltaTime, 100);
	cameraComp->SetFieldOfView(currentFOV);

	if (bRunning)
	{
		Cast<APlayerController>(GetController())->ClientPlayCameraShake(RunCameraShake);
	}
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwards", this, &ATPSPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveSideways", this, &ATPSPlayer::MoveSideways);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookSideways", this, &ATPSPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSPlayer::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSPlayer::EndCrouch);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSPlayer::Fire);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ATPSPlayer::StartZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ATPSPlayer::EndZoom);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATPSPlayer::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ATPSPlayer::EndRun);
	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &ATPSPlayer::TakeCover);
}