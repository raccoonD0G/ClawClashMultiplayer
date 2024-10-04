// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UHealthBarWidgetComponent();

protected:
	void InitWidget();

	// Health Section
protected:
	UPROPERTY()
	TWeakObjectPtr<UHealthComponent> HealthComponent;

};
