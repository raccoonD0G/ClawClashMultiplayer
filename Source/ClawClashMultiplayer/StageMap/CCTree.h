// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTree.generated.h"

UENUM()
enum class TreeState
{
	Neutral,
	RedOccupied,
	BlueOccupied
};

class UBoxComponent;
class ACCPaperPlayer;
class UPaperSpriteComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChanged, float, Percent);

UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCTree : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACCTree();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Collision Section
protected:
	UPROPERTY()
	TObjectPtr<UBoxComponent> BoxComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TObjectPtr<ACCPaperPlayer> RedPlayer;

	UPROPERTY()
	TObjectPtr<ACCPaperPlayer> BluePlayer;

public:
	FOnPercentChanged OnPercentChanged;

// Sprite Section
protected:
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> PaperSpriteComponent;

// State Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentState);
	TreeState CurrentState;

	UFUNCTION()
	void OnRep_CurrentState();

	void SetStateByRedOccupation();
public:
	void SetCurrentState(TreeState InState);

// Occupation Section
protected:
	float MaxOccupation;

	float OccupySpeed;

	UPROPERTY(ReplicatedUsing = OnRep_RedOccupation)
	float RedOccupation;

	void SetRedOccupation(float InRedOccupation);

	UFUNCTION()
	void OnRep_RedOccupation();

public:
	FORCEINLINE float GetRedOccupation() { return RedOccupation; }
	FORCEINLINE float GetBlueOccupation() { return MaxOccupation - RedOccupation; }
	FORCEINLINE float GetRedOccupationPercent() { return FMathf::Clamp(RedOccupation / MaxOccupation, 0.0f, 1.0f); }
};
