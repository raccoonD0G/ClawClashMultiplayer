// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawnManager.h"
#include "ClawClashMultiplayer/Managers/SpawnManager/CCSpawn.h"
#include "ClawClashMultiplayer/Managers/CCGameManager.h"
#include "ClawClashMultiplayer/CCUtils.h"
#include "ClawClashMultiplayer/Spawner/CCSpawner.h"

UCCSpawnManager* UCCSpawnManager::Instance = nullptr;

UCCSpawnManager::UCCSpawnManager()
{
	
}

UCCSpawnManager* UCCSpawnManager::GetInstance()
{
	if (Instance == nullptr || !Instance->IsValidLowLevel())
	{
		Instance = NewObject<UCCSpawnManager>();
		Instance->Init();
	}
	return Instance;
}

void UCCSpawnManager::Init()
{
	RatSpriteArr = UCCUtils::GetAllResourceFromFolder<UPaperFlipbook>(TEXT("/Game/Sprite/NonPlayer/Rat"));
}

const TArray<UPaperFlipbook*>& UCCSpawnManager::GetRatSpriteArr() const
{
	return RatSpriteArr;
}

