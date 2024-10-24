// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedDelegate, int32, NewCurrentHealth, int32, MaxHp);

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASHMULTIPLAYER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Init(int32 NewMaxHp);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

// Health Section
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp)
	int32 CurrentHp;

	UFUNCTION()
	void OnRep_CurrentHp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hp", ReplicatedUsing = OnRep_MaxHp)
	int32 MaxHp;

	UFUNCTION()
	void OnRep_MaxHp();

public:
	FHealthChangedDelegate OnHealthChanged;

	void GetDamaged(int32 Amount, AActor* DamageCause);

	void GetHeal(int32 Amount);

	FORCEINLINE int32 GetMaxHp() { return MaxHp; }

	FORCEINLINE float GetHealthRatio() { return (float)CurrentHp / ((MaxHp == 0) ? 1.0f : (float)MaxHp); }
};
