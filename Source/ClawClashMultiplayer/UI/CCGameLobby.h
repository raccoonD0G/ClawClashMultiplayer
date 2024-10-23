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

	UPROPERTY()
	TObjectPtr<class ACCEntryGameState> EntryGameState;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPlayerCountChange(int32 PlayerCount);
};