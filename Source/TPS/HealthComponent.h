// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, DeltaHealth, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Properties")
	float MaxHealth = 100;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
	float Health;

	// handler
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// regen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Properties")
	float HealthRegenCountdownTime;
	UPROPERTY(BlueprintReadOnly, Category = "Health Properties")
	float HealthRegenTimer;
	UFUNCTION(BlueprintCallable)
	float RegenTimeCountingDown(float DeltaTime);

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	UFUNCTION(BlueprintCallable)
	void ResetHealthRegenTimer();
	UFUNCTION(BlueprintCallable)
	void RegenHealth(float AmountPerTick);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
