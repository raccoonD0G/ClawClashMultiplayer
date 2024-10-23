// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameState/CCBattleGameState.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"

void ACCBattleGameState::BeginPlay()
{
	Super::BeginPlay();

	if (LoadingWidget)
	{
		UCCUIManager::GetInstance()->AddPopupWidget(GetWorld(), LoadingWidget);
	}
}