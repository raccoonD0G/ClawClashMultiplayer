// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCLevelWidget.h"
#include "CCBattleWidget.generated.h"


class UProgressBar;
class UExpComponent;
class UTextBlock;
class ACCPaperPlayer;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCBattleWidget : public UCCLevelWidget
{
	GENERATED_BODY()
	
public:
	void Init(UExpComponent* ExpComponent);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY()
	TObjectPtr<UExpComponent> ExpComp;

	void NativeConstruct();

	UFUNCTION()
	void OnLevelChange(int32 NewLevel);

	UFUNCTION()
	void OnExpChange(int32 NewExp);
};
