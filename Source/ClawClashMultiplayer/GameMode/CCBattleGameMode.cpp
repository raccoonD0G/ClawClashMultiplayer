// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameMode/CCBattleGameMode.h"
#include "ClawClashMultiplayer/Character/Player/CCPlayerController.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"

ACCBattleGameMode::ACCBattleGameMode()
{
	bUseSeamlessTravel = true;
}

void ACCBattleGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	ACCTeamPlayerState* OldPlayerState = Cast<ACCTeamPlayerState>(C->PlayerState);
    Super::HandleSeamlessTravelPlayer(C);
	ACCTeamPlayerState* NewPlayerState = Cast<ACCTeamPlayerState>(C->PlayerState);
	NewPlayerState->SetTeam(OldPlayerState->GetTeam());
}
