// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCOccupyIndicator.h"
#include "ClawClashMultiplayer/StageMap/CCTree.h"
#include "Components/ProgressBar.h"

void UCCOccupyIndicator::Init(ACCTree* InTree)
{
	TargetTree = InTree;
	TargetTree->OnPercentChanged.AddDynamic(this, &UCCOccupyIndicator::SetOccupyIndicatorPercent);
}

void UCCOccupyIndicator::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCCOccupyIndicator::SetOccupyIndicatorPercent(float Percent)
{
	OccupyIndicator->SetPercent(Percent);
}
