// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTree.generated.h"

enum class TreeState
{
	Neutral,
	RedOccupied,
	BlueOccupied
};

class UBoxComponent;
class ACCPaperPlayer;

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

// State Section
protected:
	TreeState CurrentState;

public:
	void SetCurrentState(TreeState InState);

// Occupation Section
protected:
	float MaxOccupation;

	UPROPERTY(ReplicatedUsing = OnRep_RedOccupation)
	float RedOccupation;

	UFUNCTION()
	void OnRep_RedOccupation();

public:
	FORCEINLINE int32 GetRedOccupation() { return RedOccupation; }
	FORCEINLINE int32 GetBlueOccupation() { return MaxOccupation - RedOccupation; }
	FORCEINLINE int32 GetRedOccupationPercent() { return FMathf::Clamp(RedOccupation / MaxOccupation, 0.0f, 1.0f); }
};
