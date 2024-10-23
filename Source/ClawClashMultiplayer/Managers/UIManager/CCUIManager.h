// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCUIManager.generated.h"

class UCCPopupWidget;
class UCCLevelWidget;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCUIManager : public UObject
{
	GENERATED_BODY()
	
	
protected:
	static UCCUIManager* Instance;

public:
	static UCCUIManager* GetInstance();

// Level Section
protected:
	UPROPERTY()
	TObjectPtr<UCCLevelWidget> LevelWidget;

public:
	void OpenLevelWidget(UWorld* InWorld, TSubclassOf<UCCPopupWidget> NewLevelWidget);

// Popup Section
protected:
	UPROPERTY()
	TArray<TObjectPtr<UCCPopupWidget>> PopupWidgets;

public:
	UCCPopupWidget* AddPopupWidget(UWorld* InWorld, TSubclassOf<UCCPopupWidget> NewPopup);
	bool RemoveTopPopupWidget(UCCPopupWidget* TopPopup);

};
