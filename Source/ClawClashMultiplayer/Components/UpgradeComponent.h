// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeComponent.generated.h"

class UExpComponent;
class ACCPaperPlayer;
class UCCPopupWidget;

UENUM()
enum class UpgradeType
{
	MoveSpeed,
	JumpPower,
	AttackPower,
	AttackRange,
	Exp,
	OccupySpeed
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASHMULTIPLAYER_API UUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpgradeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Owner Section
protected:
	UPROPERTY()
	TObjectPtr<ACCPaperPlayer> Player;

// Component Section
protected:
	UPROPERTY()
	TObjectPtr<UExpComponent> ExpComponent;

// Upgrade Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCCPopupWidget> UpgradePopup;

	UPROPERTY(Replicated)
	int32 MoveSpeedLevel;

	UPROPERTY(Replicated)
	int32 JumpPowerLevel;

	UPROPERTY(Replicated)
	int32 AttackPowerLevel;

	UPROPERTY(Replicated)
	int32 AttackRangeLevel;

	UPROPERTY(Replicated)
	int32 ExpLevel;

	UPROPERTY(Replicated)
	int32 OccupySpeedLevel;

	UPROPERTY(Replicated)
	float MoveSpeedMultiplier;

	UPROPERTY(Replicated)
	float JumpPowerMultiplier;

	UPROPERTY(Replicated)
	float AttackPowerMultiplier;

	UPROPERTY(Replicated)
	float AttackRangeMultiplier;

	UPROPERTY(Replicated)
	float ExpMultiplier;

	UPROPERTY(Replicated)
	float OccupySpeedMultiplier;

	FORCEINLINE void SetMoveSpeedMultiplier(float InMoveSpeedMultiplier) { MoveSpeedMultiplier = InMoveSpeedMultiplier; }
	FORCEINLINE void SetJumpPowerMultiplier(float InJumpPowerMultiplier) { JumpPowerMultiplier = InJumpPowerMultiplier; }
	FORCEINLINE void SetAttackPowerMultiplier(float InAttackPowerMultiplier) { AttackPowerMultiplier = InAttackPowerMultiplier; }
	FORCEINLINE void SetAttackRangeMultiplier(float InAttackRangeMultiplier) { AttackRangeMultiplier = InAttackRangeMultiplier; }
	FORCEINLINE void SetExpMultiplier(float InExpMultiplier) { ExpMultiplier = InExpMultiplier; }
	FORCEINLINE void SetOccupySpeedMultiplier(float InOccupySpeedMultiplier) { OccupySpeedMultiplier = InOccupySpeedMultiplier; }

	UFUNCTION(Server, Reliable)
	void Server_SetMoveSpeedLevel(int32 InMoveSpeedLevel);
	void Server_SetMoveSpeedLevel_Implementation(int32 InMoveSpeedLevel);
	
	UFUNCTION(Server, Reliable)
	void Server_SetJumpPowerLevel(int32 InJumpPowerLevel);
	void Server_SetJumpPowerLevel_Implementation(int32 InJumpPowerLevel);

	UFUNCTION(Server, Reliable)
	void Server_SetAttackPowerLevel(int32 InAttackPowerLevel);
	void Server_SetAttackPowerLevel_Implementation(int32 InAttackPowerLevel);

	UFUNCTION(Server, Reliable)
	void Server_SetAttackRangeLevel(int32 InAttackRangeLevel);
	void Server_SetAttackRangeLevel_Implementation(int32 InAttackRangeLevel);

	UFUNCTION(Server, Reliable)
	void Server_SetExpLevel(int32 InExpLevel);
	void Server_SetExpLevel_Implementation(int32 InExpLevel);

	UFUNCTION(Server, Reliable)
	void Server_SetOccupySpeedLevel(int32 InOccupySpeedLevel);
	void Server_SetOccupySpeedLevel_Implementation(int32 InOccupySpeedLevel);

	UFUNCTION()
	void ShowUpgradePopup(int32 NewLevel);

	TMap<UpgradeType, TFunction<void()>> UpgradeMap;

public:
	FORCEINLINE int32 GetMoveSpeedLevel() { return MoveSpeedLevel; }
	FORCEINLINE int32 GetJumpPowerLevel() { return JumpPowerLevel; }
	FORCEINLINE int32 GetAttackPowerLevel() { return AttackPowerLevel; }
	FORCEINLINE int32 GetAttackRangeLevel() { return AttackRangeLevel; }
	FORCEINLINE int32 GetExpLevel() { return ExpLevel; }
	FORCEINLINE int32 GetOccupySpeedLevel() { return OccupySpeedLevel; }

	FORCEINLINE float GetMoveSpeedMultiplier() const { return MoveSpeedMultiplier; }
	FORCEINLINE float GetJumpPowerMultiplier() const { return JumpPowerMultiplier; }
	FORCEINLINE float GetAttackPowerMultiplier() const { return AttackPowerMultiplier; }
	FORCEINLINE float GetAttackRangeMultiplier() const { return AttackRangeMultiplier; }
	FORCEINLINE float GetExpMultiplier() const { return ExpMultiplier; }
	FORCEINLINE float GetOccupySpeedMultiplier() const { return OccupySpeedMultiplier; }

	TFunction<void()>* GetUpgradeFunc(UpgradeType InUpgradeType);
};
