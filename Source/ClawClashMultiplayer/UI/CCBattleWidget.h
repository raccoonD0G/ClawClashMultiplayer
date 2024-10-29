// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCLevelWidget.h"
#include "CCBattleWidget.generated.h"


class UProgressBar;
class UExpComponent;
class UTextBlock;
class ACCPaperPlayer;
class UImage;

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BlueTreeCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RedTreeCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MiniMap;

	UPROPERTY()
	TObjectPtr<UExpComponent> ExpComp;

	void NativeConstruct();

	UFUNCTION()
	void OnLevelChange(int32 NewLevel);

	UFUNCTION()
	void OnExpChange(int32 NewExp);

	UFUNCTION()
	void OnCountChange(int32 RedCount, int32 BlueCount, int32 NeutralCount);

// Timer Section
public:
	UFUNCTION()
	void SetTimeText(int32 NewRemainingGameTime);

// Minimap Section
public:
	UFUNCTION()
	void ShowMiniMap();

	UFUNCTION()
	void HideMiniMap();
};
