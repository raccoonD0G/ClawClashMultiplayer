// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "CCUpgradeWidget.generated.h"

class UButton;
class UTextBlock;
class UUpgradeComponent;
enum class UpgradeType;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCUpgradeWidget : public UCCPopupWidget
{
	GENERATED_BODY()
	
public:
    void Init(UUpgradeComponent* InUpgradeComponent);

protected:
    UPROPERTY()
    TObjectPtr<UUpgradeComponent> UpgradeComponent;

    TArray<UpgradeType> Upgrades;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UpgradeButton0;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UpgradeButton1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UpgradeButton2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UpgradeText0;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UpgradeText1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UpgradeText2;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnUpgradeButton0Clicked();

    UFUNCTION()
    void OnUpgradeButton1Clicked();

    UFUNCTION()
    void OnUpgradeButton2Clicked();
};
