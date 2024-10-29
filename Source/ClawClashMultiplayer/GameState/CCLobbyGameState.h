// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CCLobbyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountChanged, int32, PlayerCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReadyChanged, bool, bIsRedPlayerReady, bool, bIsBluePlayerReady);

class UCCPopupWidget;
class UCCGameLobby;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	
public:
	ACCLobbyGameState();

	FORCEINLINE int32 GetPlayerCount() { return PlayerCount; }
	FORCEINLINE void AddPlayerCount() { PlayerCount++; }

	void SetRedPlayerReady(bool InbIsRedPlayerReady);
	void SetBluePlayerReady(bool InbIsBluePlayerReady);

	FOnPlayerCountChanged OnPlayerCountChangedEvent;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 PlayerCount;

	UFUNCTION()
	void OnRep_PlayerCount();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCCPopupWidget> LobbyWidgetClass;

	UPROPERTY()
	TObjectPtr<UCCGameLobby> LobbyWidget;

	UPROPERTY(ReplicatedUsing = OnRep_bIsBluePlayerReady)
	bool bIsBluePlayerReady;

	UFUNCTION()
	void OnRep_bIsBluePlayerReady();

	UPROPERTY(ReplicatedUsing = OnRep_bIsRedPlayerReady)
	bool bIsRedPlayerReady;

	UFUNCTION()
	void OnRep_bIsRedPlayerReady();

public:
	FOnReadyChanged OnReadyChangedEvent;
	
};
