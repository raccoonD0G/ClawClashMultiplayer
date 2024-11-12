// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCResaultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCResaultPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACCResaultPlayerController();

protected:
	void BeginPlay();

};
