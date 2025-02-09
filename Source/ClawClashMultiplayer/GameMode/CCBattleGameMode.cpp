// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameMode/CCBattleGameMode.h"
#include "ClawClashMultiplayer/Character/Player/CCPlayerController.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/CCPlayerSpawner.h"
#include "ClawClashMultiplayer/Managers/TreeManager/CCTreeManager.h"
#include "ClawClashMultiplayer/GameInstance/CCGameInstance.h"
#include <ClawClashMultiplayer/Managers/CCGameManager.h>
#include <ClawClashMultiplayer/Managers/SpawnManager/CCSpawnManager.h>
#include <ClawClashMultiplayer/Managers/UIManager/CCUIManager.h>
#include <ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h>

ACCBattleGameMode::ACCBattleGameMode()
{
	bUseSeamlessTravel = true;
}

void ACCBattleGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACCBattleGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	ACCTeamPlayerState* OldPlayerState = Cast<ACCTeamPlayerState>(C->PlayerState);
    Super::HandleSeamlessTravelPlayer(C);
	ACCTeamPlayerState* NewPlayerState = Cast<ACCTeamPlayerState>(C->PlayerState);
	NewPlayerState->SetTeam(OldPlayerState->GetTeam());
}

void ACCBattleGameMode::EndMatch()
{
	UE_LOG(LogTemp, Log, TEXT("EndMatch"));
	int32 RedScore = UCCTreeManager::GetInstance()->GetRedTreeCount();
	int32 BlueScore = UCCTreeManager::GetInstance()->GetBlueTreeCount();

	UCCGameInstance* GameInstance = Cast<UCCGameInstance>(GetGameInstance());
	if (RedScore > BlueScore)
	{
		UE_LOG(LogTemp, Log, TEXT("Red"));
		GameInstance->SetWinnerTeam(EPlayerTeam::Red);
	}
	else if (RedScore < BlueScore)
	{
		UE_LOG(LogTemp, Log, TEXT("Blue"));
		GameInstance->SetWinnerTeam(EPlayerTeam::Blue);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("None"));
		GameInstance->SetWinnerTeam(EPlayerTeam::None);
	}

	GameInstance->SetBlueScore(BlueScore);
	GameInstance->SetRedScore(RedScore);

	UCCGameManager::DestroyInstance();
	UCCTreeManager::DestroyInstance();
	UCCStageMapManager::DestroyInstance();
	UCCSpawnManager::DestroyInstance();
	UCCUIManager::DestroyInstance();

	FString LevelName = TEXT("/Game/Maps/GameResaultLevel");
	GetWorld()->ServerTravel(LevelName, true);
}