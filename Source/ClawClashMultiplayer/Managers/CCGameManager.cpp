// Fill out your copyright notice in the Description page of Project Settings.


#include "CCGameManager.h"
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/Managers/SpawnManager/CCSpawnManager.h"
#include "CCGameManager.h"


UCCGameManager* UCCGameManager::Instance;


UCCGameManager* UCCGameManager::GetInstance()
{
    if (Instance == nullptr || !Instance->IsValidLowLevel())
    {
        Instance = NewObject<UCCGameManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

void UCCGameManager::DestroyInstance()
{
    if (Instance)
    {
        Instance->RemoveFromRoot();
        Instance = nullptr;
    }
}



