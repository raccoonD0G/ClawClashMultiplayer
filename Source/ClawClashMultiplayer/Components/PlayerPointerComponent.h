// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PlayerPointerComponent.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UPlayerPointerComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	void SetPlayerPointerColor();

	FTimerHandle InitHandle;
};
