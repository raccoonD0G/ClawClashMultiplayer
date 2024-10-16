// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCUpgradeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ClawClashMultiplayer/CCUtils.h"
#include "ClawClashMultiplayer/Components/UpgradeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"

void UCCUpgradeWidget::Init(UUpgradeComponent* InUpgradeComponent)
{
    UpgradeComponent = InUpgradeComponent;
    TSet<UpgradeType> UpgradeSet;
    while (UpgradeSet.Num() < 3)
    {
        UpgradeSet.Add(static_cast<UpgradeType>(FMath::RandRange(0, UCCUtils::GetEnumLength(StaticEnum<UpgradeType>()) - 1)));
    }

    Upgrades = UpgradeSet.Array();

    if (UpgradeButton0)
    {
        UpgradeButton0->OnClicked.AddDynamic(this, &UCCUpgradeWidget::OnUpgradeButton0Clicked);
    }

    if (UpgradeButton1)
    {
        UpgradeButton1->OnClicked.AddDynamic(this, &UCCUpgradeWidget::OnUpgradeButton1Clicked);
    }

    if (UpgradeButton2)
    {
        UpgradeButton2->OnClicked.AddDynamic(this, &UCCUpgradeWidget::OnUpgradeButton2Clicked);
    }

    if (UpgradeText0)
    {
        UpgradeText0->SetText(FText::FromString(StaticEnum<UpgradeType>()->GetNameStringByIndex(static_cast<int32>(Upgrades[0]))));
    }
    if (UpgradeText1)
    {
        UpgradeText1->SetText(FText::FromString(StaticEnum<UpgradeType>()->GetNameStringByIndex(static_cast<int32>(Upgrades[1]))));
    }
    if (UpgradeText2)
    {
        UpgradeText2->SetText(FText::FromString(StaticEnum<UpgradeType>()->GetNameStringByIndex(static_cast<int32>(Upgrades[2]))));
    }
}

void UCCUpgradeWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCCUpgradeWidget::OnUpgradeButton0Clicked()
{
    (*UpgradeComponent->GetUpgradeFunc(Upgrades[0]))();
    UCCUIManager::GetInstance()->RemoveTopPopupWidget(this);
}

void UCCUpgradeWidget::OnUpgradeButton1Clicked()
{
    (*UpgradeComponent->GetUpgradeFunc(Upgrades[1]))();
    UCCUIManager::GetInstance()->RemoveTopPopupWidget(this);
}

void UCCUpgradeWidget::OnUpgradeButton2Clicked()
{
    (*UpgradeComponent->GetUpgradeFunc(Upgrades[2]))();
    UCCUIManager::GetInstance()->RemoveTopPopupWidget(this);
}
