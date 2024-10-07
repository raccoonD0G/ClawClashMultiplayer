// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerController.h"
#include "CCPaperPlayer.h"
#include "EngineUtils.h"

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