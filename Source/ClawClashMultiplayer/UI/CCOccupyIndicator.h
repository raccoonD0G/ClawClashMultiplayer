// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCWorldSpaceWidget.h"
#include "CCOccupyIndicator.generated.h"

class UProgressBar;
class ACCTree;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCOccupyIndicator : public UCCWorldSpaceWidget
{
	GENERATED_BODY()
	
public:
	void Init(ACCTree* InTree);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<ACCTree> TargetTree;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> OccupyIndicator;

	UFUNCTION()
	void SetOccupyIndicatorPercent(float Percent);
};
