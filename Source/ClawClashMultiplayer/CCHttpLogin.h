// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "CCHttpLogin.generated.h"

enum class EPlayerTeam : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginFail);

UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCHttpLogin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCHttpLogin();

public:
	UFUNCTION(Reliable, Server)
	void Server_SendLoginRequest(const FString& InUserId, const FString& UserPassword, EPlayerTeam InTeam);
	void Server_SendLoginRequest_Implementation(const FString& InUserId, const FString& UserPassword, EPlayerTeam InTeam);

	FOnLoginSuccess OnLoginSuccessEvent;
	FOnLoginFail OnLoginFailEvent;

protected:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString UserId;
	EPlayerTeam Team;

};
