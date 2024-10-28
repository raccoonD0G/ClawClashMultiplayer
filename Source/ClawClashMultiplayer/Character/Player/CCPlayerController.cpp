// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerController.h"
#include "CCPaperPlayer.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "ClawClashMultiplayer/UI/CCBattleWidget.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/CCPlayerSpawner.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include <ClawClashMultiplayer/GameMode/CCLobbyGameMode.h>

ACCPlayerController::ACCPlayerController()
{
	bReplicates = true;
}

void ACCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void ACCPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();
}

void ACCPlayerController::Server_OnReadyButtonClicked_Implementation()
{
	if (GetPlayerState<ACCTeamPlayerState>()->GetTeam() == EPlayerTeam::Blue)
	{
		ACCLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ACCLobbyGameMode>();
		GameMode->SetBluePlayerReady();
	}
	else
	{
		ACCLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ACCLobbyGameMode>();
		GameMode->SetRedPlayerReady();
	}
}
