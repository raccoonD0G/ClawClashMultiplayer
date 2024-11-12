// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCPlayerController.generated.h"

class UUserWidget;
class UCCBattleWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepPawn);


/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACCPlayerController();

protected:
	virtual void BeginPlay() override;
};