// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerController.h"


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