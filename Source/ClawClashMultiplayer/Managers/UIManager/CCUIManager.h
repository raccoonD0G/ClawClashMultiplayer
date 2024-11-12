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
	static void DestroyInstance();

// Level Section
protected:
	UPROPERTY()
	TObjectPtr<UCCLevelWidget> LevelWidget;

public:
	UCCLevelWidget* OpenLevelWidget(UWorld* InWorld, TSubclassOf<UCCLevelWidget> NewLevelWidget);
	FORCEINLINE UCCLevelWidget* GetLevelWidget() const { return LevelWidget; }

// Popup Section
protected:
	int32 ZOrder = 1;

	UPROPERTY()
	TArray<TObjectPtr<UCCPopupWidget>> PopupWidgets;

public:
	UCCPopupWidget* AddPopupWidget(UWorld* InWorld, TSubclassOf<UCCPopupWidget> NewPopup);
	bool RemoveTopPopupWidget(UCCPopupWidget* TopPopup);

};
