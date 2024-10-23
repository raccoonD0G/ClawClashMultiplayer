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
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCCPopupWidget> LoadingWidget;
};
