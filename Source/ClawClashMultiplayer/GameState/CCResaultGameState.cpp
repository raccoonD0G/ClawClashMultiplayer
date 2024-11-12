// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameState/CCResaultGameState.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCResaultWidget.h"
#include "Net/UnrealNetwork.h"

ACCResaultGameState::ACCResaultGameState()
{
	bReplicates = true;
	WinnerTeam = EPlayerTeam::Blue;
}

void ACCResaultGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACCResaultGameState, WinnerTeam);
}

void ACCResaultGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!HasAuthority())
	{
		if (GameResultWidget)
		{
			UCCLevelWidget* LevelWidget = UCCUIManager::GetInstance()->OpenLevelWidget(GetWorld(), GameResultWidget);
			if (LevelWidget)
			{
				ResaultWidget = Cast<UCCResaultWidget>(LevelWidget);
			}
		}
	}
}

void ACCResaultGameState::OnRep_WinnerTeam()
{
	ResaultWidget->Init(WinnerTeam);
}
