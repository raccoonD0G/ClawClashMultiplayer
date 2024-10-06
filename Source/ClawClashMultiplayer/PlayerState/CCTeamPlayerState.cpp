// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "Net/UnrealNetwork.h"

ACCTeamPlayerState::ACCTeamPlayerState()
{
	bReplicates = true;
}

void ACCTeamPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCTeamPlayerState, Team);
}

void ACCTeamPlayerState::OnRep_Team()
{
}
