// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CCBattleGameState.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCBattleGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCCPopupWidget> LoadingWidgetClass;

	UPROPERTY()
	TObjectPtr<class UCCLoadingMapWidget> LoadingMapWidget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCCLevelWidget> BattleWidgetClass;

	UPROPERTY()
	TObjectPtr<class UCCBattleWidget> BattleWidget;
};
