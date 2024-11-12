// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCLevelWidget.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "CCResaultWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCResaultWidget : public UCCLevelWidget
{
	GENERATED_BODY()
	
public:
	void Init(EPlayerTeam InWinnerTeam);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UTextBlock> GameResaultText;

	UPROPERTY()
	TObjectPtr<class UButton> QuitButton;

	UFUNCTION()
	void OnQuitButtonClicked();

	EPlayerTeam WinnerTeam;
	
};
