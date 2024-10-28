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

ACCPlayerController::ACCPlayerController()
{
	bReplicates = true;
}

void ACCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void ACCPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();
}
