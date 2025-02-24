// Fill out your copyright notice in the Description page of Project Settings.


#include "CCGameInstance.h"
#include "GameFramework/GameUserSettings.h"

#include "ClawClashMultiplayer/Managers/CCGameManager.h"
#include "ClawClashMultiplayer/UI/CCTimerWidget.h"
#include "Blueprint/UserWidget.h"
#include <ClawClashMultiplayer/Managers/TreeManager/CCTreeManager.h>
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/Managers/SpawnManager/CCSpawnManager.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"


UCCGameInstance::UCCGameInstance()
{
}

void UCCGameInstance::Init()
{
    Super::Init();
}

void UCCGameInstance::OnStart()
{
    Super::OnStart();

    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    UserSettings->SetScreenResolution(FIntPoint(1920, 1200));
    UserSettings->ApplySettings(true);
}

void UCCGameInstance::Shutdown()
{
    Super::Shutdown();

    UCCGameManager::DestroyInstance();
    UCCTreeManager::DestroyInstance();
    UCCStageMapManager::DestroyInstance();
    UCCSpawnManager::DestroyInstance();
    UCCUIManager::DestroyInstance();
}

void UCCGameInstance::AddID(EPlayerTeam Team, FString ID)
{
    TeamIDMap.Add(Team, ID);
}

void UCCGameInstance::RemoveID(EPlayerTeam Team)
{
    TeamIDMap.FindAndRemoveChecked(Team);
}

void UCCGameInstance::ResetGameInstance()
{
    TeamIDMap.Empty();

    WinnerTeam = EPlayerTeam::None;
    BlueScore = 0;
    RedScore = 0;
}
