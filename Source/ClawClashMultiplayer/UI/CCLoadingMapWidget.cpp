// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCLoadingMapWidget.h"
#include "Components/ProgressBar.h"
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"

void UCCLoadingMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	LoadingProgressBar->SetPercent(UCCStageMapManager::GetInstance()->MapLoadingPercent());
	if (UCCStageMapManager::GetInstance()->IsTileMapGenerated())
	{
		FTimerHandle LoadingEndTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LoadingEndTimerHandle, this, &UCCLoadingMapWidget::OnLoadingEnd, 0.2f, false);
	}
}

void UCCLoadingMapWidget::OnLoadingEnd()
{
	UCCUIManager::GetInstance()->RemoveTopPopupWidget(this);
	OnLoadingEndEvent.Broadcast();
}
