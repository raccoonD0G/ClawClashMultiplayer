// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CCEntryGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountChanged, int32, PlayerCount);

class UCCPopupWidget;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCEntryGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	
public:
	ACCEntryGameState();

	FORCEINLINE int32 GetPlayerCount() { return PlayerCount; }
	FORCEINLINE void AddPlayerCount() { PlayerCount++; }

	FOnPlayerCountChanged OnPlayerCountChangedEvent;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 PlayerCount;

	UFUNCTION()
	void OnRep_PlayerCount();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCCPopupWidget> LobbyWidgetClass;
	
};
