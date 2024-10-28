// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameState/CCBattleGameState.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "ClawClashMultiplayer/UI/CCBattleWidget.h"
#include "ClawClashMultiplayer/UI/CCLevelWidget.h"
#include <ClawClashMultiplayer/Character/Player/CCPlayerController.h>
#include "ClawClashMultiplayer/UI/CCLoadingMapWidget.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include <ClawClashMultiplayer/CCTimer.h>

void ACCBattleGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FVector Location = FVector(0.0f, 0.0f, 0.0f);
		FRotator Rotation = FRotator::ZeroRotator;

		ACCTimer* Timer = GetWorld()->SpawnActor<ACCTimer>(ACCTimer::StaticClass(), Location, Rotation);
		Timer->Init(BattleWidget);
	}
}

void ACCBattleGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!HasAuthority())
	{
		if (BattleWidgetClass)
		{
			BattleWidget = Cast<UCCBattleWidget>(UCCUIManager::GetInstance()->OpenLevelWidget(GetWorld(), BattleWidgetClass));
		}

		if (LoadingWidgetClass)
		{
			LoadingMapWidget = Cast<UCCLoadingMapWidget>(UCCUIManager::GetInstance()->AddPopupWidget(GetWorld(), LoadingWidgetClass));
		}
	}
}
