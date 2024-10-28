// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerEnd);

UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCTimer();
	void Init(class UCCBattleWidget* NewBattleWidget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Timer Section
protected:
	FTimerHandle GameTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalGameTime;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingGameTime)
	int32 RemainingGameTime;

	UFUNCTION()
	void OnRep_RemainingGameTime();

	void UpdateGameTimer();

	void EndGame();

	UPROPERTY()
	TObjectPtr<class UCCBattleWidget> BattleWidget;

public:
	FOnTimerEnd OnTimerEndEvent;
};
