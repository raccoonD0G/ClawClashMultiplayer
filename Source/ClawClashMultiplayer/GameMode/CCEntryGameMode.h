// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/GameMode/CCGameModeBase.h"
#include "CCEntryGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCEntryGameMode : public ACCGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

// Player Section
protected:
	UPROPERTY()
	TObjectPtr<APlayerController> RedPlayerController;

	UPROPERTY()
	TObjectPtr<APlayerController> BluePlayerController;

protected:
	UFUNCTION()
	void StartMatch();

	FTimerHandle Handle;
	
};
