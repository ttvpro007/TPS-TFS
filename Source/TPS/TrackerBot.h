// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackerBot.generated.h"
class USoundCue;
class USphereComponent;
class UHealthComponent;
UCLASS()
class TPS_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	UHealthComponent* HealthComp;

	FVector CalculateNextPathPoint();
	FVector NextPathPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float MoveForce = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	bool bUseVelocityChange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float closingDistance = 100;

	// Health Changed
	UFUNCTION()
	void HandleOnHealthChanged(UHealthComponent* HealthCompVar, float Health, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UMaterialInstanceDynamic* MatInst;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	UParticleSystem* explodeEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	USoundCue* explodeSoundEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float ExplosionDamage = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float ExplodeRadius = 500;
	bool bExploded = false;
	void SelfDestruct();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Components")
	USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float destructionTriggerRadius = 300;
	UFUNCTION()
	void StartSelfDamage(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FTimerHandle selfDamageTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float selfDamageTimeInterval = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	float selfDamage = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Properties")
	USoundCue* damageSoundEffect;
	void DamageSelf();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
