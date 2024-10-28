// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "CCLoadingMapWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingEnd);

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCLoadingMapWidget : public UCCPopupWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> LoadingProgressBar;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnLoadingEnd();

public:
	FOnLoadingEnd OnLoadingEndEvent;
	
};
