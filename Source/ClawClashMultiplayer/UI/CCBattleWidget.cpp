// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCBattleWidget.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "ClawClashMultiplayer/Managers/TreeManager/CCTreeManager.h"

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

	UCCTreeManager::GetInstance()->OnCountUpdateEvent.AddDynamic(this, &UCCBattleWidget::OnCountChange);
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

void UCCBattleWidget::OnCountChange(int32 RedCount, int32 BlueCount, int32 NeutralCount)
{
	RedTreeCountText->SetText(FText::FromString(*FString::Printf(TEXT("%d"), RedCount)));
	BlueTreeCountText->SetText(FText::FromString(*FString::Printf(TEXT("%d"), BlueCount)));
}

void UCCBattleWidget::SetTimeText(int32 NewRemainingGameTime)
{
	int32 Min = NewRemainingGameTime / 60;
	int32 Sec = NewRemainingGameTime % 60;

	TimeText->SetText(FText::FromString(FString::Printf(TEXT("%d:%d"), Min, Sec)));
}
