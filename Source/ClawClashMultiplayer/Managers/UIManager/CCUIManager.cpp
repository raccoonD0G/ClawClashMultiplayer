// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "ClawClashMultiplayer/UI/CCLevelWidget.h"

UCCUIManager* UCCUIManager::Instance = nullptr;

UCCUIManager* UCCUIManager::GetInstance()
{
	if (Instance == nullptr || !Instance->IsValidLowLevel())
	{
		Instance = NewObject<UCCUIManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

void UCCUIManager::OpenLevelWidget(UWorld* InWorld, TSubclassOf<UCCPopupWidget> NewLevelWidget)
{
	if (LevelWidget != nullptr && LevelWidget->IsValidLowLevel())
	{
		LevelWidget->RemoveFromParent();
		LevelWidget = nullptr;
	}

	LevelWidget = CreateWidget<UCCLevelWidget>(InWorld, NewLevelWidget);
	LevelWidget->AddToViewport();
}

UCCPopupWidget* UCCUIManager::AddPopupWidget(UWorld* InWorld, TSubclassOf<UCCPopupWidget> NewPopup)
{
	if (NewPopup)
	{
		UCCPopupWidget* Popup = CreateWidget<UCCPopupWidget>(InWorld, NewPopup);
		Popup->AddToViewport();
		PopupWidgets.Add(Popup);
		return Popup;
	}
	return nullptr;
}

bool UCCUIManager::RemoveTopPopupWidget(UCCPopupWidget* TopPopup)
{
	if (PopupWidgets.IsEmpty())
	{
		return false;
	}
	if (PopupWidgets[PopupWidgets.Num() - 1] == TopPopup)
	{
		TopPopup->RemoveFromParent();
		PopupWidgets.RemoveAt(PopupWidgets.Num() - 1);
		return true;
	}
	return false;
}
