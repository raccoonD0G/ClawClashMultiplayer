// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Managers/TreeManager/CCTreeManager.h"
#include "ClawClashMultiplayer/StageMap/CCTree.h"

UCCTreeManager* UCCTreeManager::Instance = nullptr;

UCCTreeManager* UCCTreeManager::GetInstance()
{
	if (!Instance || !Instance->IsValidLowLevel())
	{
		Instance = NewObject<UCCTreeManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

void UCCTreeManager::DestroyInstance()
{
	if (Instance)
	{
		Instance->RemoveFromRoot();
		Instance = nullptr;
	}
}

void UCCTreeManager::AddRedTree(ACCTree* NewRedTree)
{
	if (!NewRedTree->IsValidLowLevel())
	{
		return;
	}
	
	RedTrees.Emplace(NewRedTree);
	BlueTrees.Remove(NewRedTree);
	NeutralTrees.Remove(NewRedTree);
	OnCountUpdateEvent.Broadcast(RedTrees.Num(), BlueTrees.Num(), NeutralTrees.Num());
}

void UCCTreeManager::AddBlueTree(ACCTree* NewBlueTree)
{
	if (!NewBlueTree->IsValidLowLevel())
	{
		return;
	}

	RedTrees.Remove(NewBlueTree);
	BlueTrees.Emplace(NewBlueTree);
	NeutralTrees.Remove(NewBlueTree);
	OnCountUpdateEvent.Broadcast(RedTrees.Num(), BlueTrees.Num(), NeutralTrees.Num());
}

void UCCTreeManager::AddNeutralTree(ACCTree* NewNeutralTree)
{
	if (!NewNeutralTree->IsValidLowLevel())
	{
		return;
	}

	RedTrees.Remove(NewNeutralTree);
	BlueTrees.Remove(NewNeutralTree);
	NeutralTrees.Emplace(NewNeutralTree);
	OnCountUpdateEvent.Broadcast(RedTrees.Num(), BlueTrees.Num(), NeutralTrees.Num());
}
