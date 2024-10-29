// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameState/CCLobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "ClawClashMultiplayer/UI/CCGameLobby.h"

void ACCLobbyGameState::BeginPlay()
{
	Super::BeginPlay();
	if (LobbyWidgetClass)
	{
		LobbyWidget = Cast<UCCGameLobby>(UCCUIManager::GetInstance()->AddPopupWidget(GetWorld(), LobbyWidgetClass));
	}
}

ACCLobbyGameState::ACCLobbyGameState()
{
	PlayerCount = 0;
}

void ACCLobbyGameState::SetRedPlayerReady(bool InbIsRedPlayerReady)
{
	bIsRedPlayerReady = InbIsRedPlayerReady;
}

void ACCLobbyGameState::SetBluePlayerReady(bool InbIsBluePlayerReady)
{
	bIsBluePlayerReady = InbIsBluePlayerReady;
}

void ACCLobbyGameState::OnRep_PlayerCount()
{
	OnPlayerCountChangedEvent.Broadcast(PlayerCount);
}

void ACCLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCLobbyGameState, PlayerCount);
	DOREPLIFETIME(ACCLobbyGameState, bIsBluePlayerReady);
	DOREPLIFETIME(ACCLobbyGameState, bIsRedPlayerReady);
}

void ACCLobbyGameState::OnRep_bIsBluePlayerReady()
{
	OnReadyChangedEvent.Broadcast(bIsRedPlayerReady, bIsBluePlayerReady);
}

void ACCLobbyGameState::OnRep_bIsRedPlayerReady()
{
	OnReadyChangedEvent.Broadcast(bIsRedPlayerReady, bIsBluePlayerReady);
}
