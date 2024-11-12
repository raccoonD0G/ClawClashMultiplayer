// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/GameMode/CCGameModeBase.h"
#include "Http.h"
#include "CCResaultGameMode.generated.h"

enum class EPlayerTeam : uint8;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCResaultGameMode : public ACCGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	UFUNCTION(Reliable, Server)
	void Server_SendGameResult(const FString& UserId, const FString& Opponent, const FString& MyTeam, const FString& WinStatus, const FString& RedScore, const FString& BlueScore);
	void Server_SendGameResult_Implementation(const FString& UserId, const FString& Opponent, const FString& MyTeam, const FString& WinStatus, const FString& RedScore, const FString& BlueScore);

	void OnGameResultResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString TeamEnumToString(EPlayerTeam Team);
};
