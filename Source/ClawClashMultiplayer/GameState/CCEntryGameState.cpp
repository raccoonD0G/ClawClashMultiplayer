// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameState/CCEntryGameState.h"
#include "Net/UnrealNetwork.h"

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
