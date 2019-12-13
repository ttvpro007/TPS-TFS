// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class ATPSWeapon;
class UBoxComponent;
class UHealthComponent;

UENUM()
enum CharacterStateEnum
{
	Walking				UMETA(DisplayName = "Walking"),
	SlidingIntoCover    UMETA(DisplayName = "SlidingIntoCover"),
	InCover				UMETA(DisplayName = "InCover"),
	InCoverAndAiming	UMETA(DisplayName = "InCoverAndAiming"),
	Dead				UMETA(DisplayName = "Dead")
};

UCLASS()
class TPS_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/**/
	void MoveForward(float axis);
	void MoveSideways(float axis);
	UFUNCTION(BlueprintCallable)
	void StartCrouch();
	UFUNCTION(BlueprintCallable)
	void EndCrouch();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	TSubclassOf<ATPSWeapon> weaponType;
	ATPSWeapon* currentWeapon;
public:
	virtual FVector FireStartPos();
	virtual FVector FireForwardDirection();
protected:
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION(BlueprintCallable)
	void StartZoom();
	UFUNCTION(BlueprintCallable)
	void EndZoom();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
	bool bZoomed;

	/*RUN*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Properties")
	float defaultSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Properties")
	float runSpeed;
	float currentSpeed;
	bool bRunning;
	UFUNCTION(BlueprintCallable)
	void StartRun();
	UFUNCTION(BlueprintCallable)
	void EndRun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Properties")
	TEnumAsByte<CharacterStateEnum> PlayerCharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cover Properties")
	UBoxComponent* CoverTriggerVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover Properties")
	FVector TakeCoverStartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover Properties")
	FRotator TakeCoverStartRotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cover Properties")
	float CoverPadding;
	UFUNCTION(BlueprintCallable)
	void TakeCover();
	FVector TargetCoverPos;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cover Properties")
	float slidingIntoCoverSpeed = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cover Properties")
	float closingDistance = 10.0f;
	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	UHealthComponent* HealthComp;
	UFUNCTION()
	void HandleOnHealthChanged(UHealthComponent* HealthCompVar, float Health, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	// Material
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Properties")
	UMaterialInterface* deathMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
