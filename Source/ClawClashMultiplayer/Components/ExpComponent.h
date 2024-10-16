// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExpChange, int32, NewExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChange, int32, NewLevel);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASHMULTIPLAYER_API UExpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void AddExp(int32 InExp);
	FORCEINLINE int32 GetLevel() { return Level; }
	FORCEINLINE int32 GetExp() { return Exp; }
	FORCEINLINE int32 GetMaxExp() { return MaxExp; }
	FORCEINLINE float GetExpPercent() { return (float)Exp / (float)MaxExp; }

	FOnExpChange OnExpChange;
	FOnLevelChange OnLevelChange;

protected:
	float MaxExpMultiplier;

	UPROPERTY(ReplicatedUsing = OnRep_MaxExp, VisibleAnywhere, BlueprintReadOnly)
	int32 MaxExp;

	UPROPERTY(ReplicatedUsing = OnRep_Exp, VisibleAnywhere, BlueprintReadOnly)
	int32 Exp;

	UPROPERTY(ReplicatedUsing = OnRep_Level, VisibleAnywhere, BlueprintReadOnly)
	int32 Level;

	void LevelUp();

	UFUNCTION()
	void OnRep_MaxExp();

	UFUNCTION()
	void OnRep_Exp();

	UFUNCTION()
	void OnRep_Level();

// Upgrade Section
protected:
	TArray<TFunction<float()>> ExpMultipliers;

public:
	FORCEINLINE void AddExpMultiplier(TFunction<float()> Func) { ExpMultipliers.Add(Func); }
};
