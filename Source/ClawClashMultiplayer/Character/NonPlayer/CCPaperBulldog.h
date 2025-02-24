// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/Character/NonPlayer/CCPaperNonPlayer.h"
#include "ClawClashMultiplayer/Interfaces/CCRushable.h"
#include "ClawClashMultiplayer/Interfaces/CCReadyRushable.h"
#include "ClawClashMultiplayer/Interfaces/CCDestroyable.h"
#include "CCPaperBulldog.generated.h"

UENUM(BlueprintType)
enum class EBulldogState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	ReadyRush UMETA(DisplayName = "ReadyRush"),
	Rush UMETA(DisplayName = "Rush"),
};

class UDamageSphereComponent;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPaperBulldog : public ACCPaperNonPlayer, public ICCRushable, public ICCReadyRushable, public ICCDestroyable
{
	GENERATED_BODY()

public:
	ACCPaperBulldog();

	virtual void BeginPlay() override;

// Sprite Section
public:
	virtual void FaceDirection(FVector Dir) override;

// State Section
protected:
	EBulldogState CurrentState;

public:
	void SetCurrentState(EBulldogState NewState);
	FORCEINLINE EBulldogState GetCurrentState() { return CurrentState; }

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> ReadyRushAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> RushAnimation;

// Stat Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RushMoveSpeed;

// Rush Section
protected:
	UPROPERTY()
	TObjectPtr<UDamageSphereComponent> DamageSphereComponent;

public:
	virtual void StartReadyRush() override;
	virtual float GetReadyRushTime() override;
	virtual void StartRush() override;
	virtual float GetRushTime() override;
	virtual void EndRush() override;

// Move Section
public :
	virtual void StartMove() override;
	virtual void EndMove() override;

// Death Section
public:
	virtual void OnDeath(AActor* Destroyer) override;
};