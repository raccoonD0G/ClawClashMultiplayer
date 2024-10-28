// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "CCGameLobby.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCGameLobby : public UCCPopupWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ExplainText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RedPlayerReadyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BluePlayerReadyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TeamText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReadyButton;

	UPROPERTY()
	TObjectPtr<class ACCLobbyGameState> EntryGameState;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetReady();

	UFUNCTION()
	void OnPlayerCountChange(int32 PlayerCount);

	UFUNCTION()
	void OnReadyChange(bool bIsRedPlayerReady, bool bIsBluePlayerReady);
};