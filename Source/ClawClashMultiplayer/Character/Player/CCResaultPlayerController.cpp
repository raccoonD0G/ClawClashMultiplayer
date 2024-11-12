// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Character/Player/CCResaultPlayerController.h"

ACCResaultPlayerController::ACCResaultPlayerController()
{
	bReplicates = true;
}


void ACCResaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}