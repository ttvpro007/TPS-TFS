// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSCharacter.h"
#include "TPSPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCameraShake;
/**
 * 
 */
UCLASS()
class TPS_API ATPSPlayer : public ATPSCharacter
{
	GENERATED_BODY()
public:
	ATPSPlayer();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	UCameraComponent* cameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	USpringArmComponent* springComp;

	// FOV
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties", meta = (ClampMin = 0.1, ClampMax = 100))
	float defaultFOV;
	float currentFOV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Properties")
	TSubclassOf<UCameraShake> RunCameraShake;
public:
	virtual FVector FireStartPos() override;
	virtual FVector FireForwardDirection() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
