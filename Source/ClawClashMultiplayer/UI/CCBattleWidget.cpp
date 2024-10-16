// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCBattleWidget.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"

void UCCBattleWidget::Init(UExpComponent* ExpComponent)
{
	if (!ExpComponent)
	{
		return;
	}

	ExpComp = ExpComponent;

	FString LevelString = FString::Printf(TEXT("Lv. %d"), ExpComp->GetLevel());
	LevelText->SetText(FText::FromString(LevelString));

	ExpBar->SetPercent(ExpComp->GetExpPercent());

	ExpComp->OnExpChange.AddDynamic(this, &UCCBattleWidget::OnExpChange);
	ExpComp->OnLevelChange.AddDynamic(this, &UCCBattleWidget::OnLevelChange);
}

void UCCBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UCCBattleWidget::OnLevelChange(int32 NewLevel)
{
	FString LevelString = FString::Printf(TEXT("Lv. %d"), ExpComp->GetLevel());
	LevelText->SetText(FText::FromString(LevelString));
}

void UCCBattleWidget::OnExpChange(int32 NewExp)
{
	ExpBar->SetPercent(ExpComp->GetExpPercent());
}
