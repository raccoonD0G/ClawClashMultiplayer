// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/GameMode/CCGameModeBase.h"
#include "CCBattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCBattleGameMode : public ACCGameModeBase
{
	GENERATED_BODY()
	
public:
	ACCBattleGameMode();

protected:
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
};
