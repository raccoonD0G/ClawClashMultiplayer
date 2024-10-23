// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "CCOnDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCOnDeathWidget : public UCCPopupWidget
{
	GENERATED_BODY()
	
public:
	UCCOnDeathWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RespawnText;

	int32 TimeLeft;

	UFUNCTION()
	void Countdown();

	virtual void NativeConstruct() override;
	
};
