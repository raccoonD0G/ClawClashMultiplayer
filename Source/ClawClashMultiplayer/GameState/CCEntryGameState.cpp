// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameState/CCEntryGameState.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"

void ACCEntryGameState::BeginPlay()
{
	Super::BeginPlay();
	if (LobbyWidgetClass)
	{
		UCCUIManager::GetInstance()->AddPopupWidget(GetWorld(), LobbyWidgetClass);
	}
}

ACCEntryGameState::ACCEntryGameState()
{
	PlayerCount = 0;
}

void ACCEntryGameState::OnRep_PlayerCount()
{
	OnPlayerCountChangedEvent.Broadcast(PlayerCount);
}

void ACCEntryGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCEntryGameState, PlayerCount);
}
