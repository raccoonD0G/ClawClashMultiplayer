// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/GameMode/CCGameModeBase.h"
#include "CCLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCLobbyGameMode : public ACCGameModeBase
{
	GENERATED_BODY()
	
public:
	ACCLobbyGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

// Player Section
public:
	void SetRedPlayerReady();
	void SetBluePlayerReady();

protected:
	UPROPERTY()
	TObjectPtr<APlayerController> RedPlayerController;

	bool bIsRedPlayerReady;

	UPROPERTY()
	TObjectPtr<APlayerController> BluePlayerController;

	bool bIsBluePlayerReady;

protected:
	UFUNCTION()
	void StartMatchInSec(float Sec);

	UFUNCTION()
	void StartMatch();

	void HandleSeamlessTravelPlayer(AController*& C);

	FTimerHandle Handle;
	
};
