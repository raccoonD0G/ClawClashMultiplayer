// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/OccupyWidgetComponent.h"
#include "ClawClashMultiplayer/UI/CCOccupyIndicator.h"
#include "ClawClashMultiplayer/StageMap/CCTree.h"

void UOccupyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	ACCTree* Tree = Cast<ACCTree>(GetOwner());
	if (Tree)
	{
		UCCOccupyIndicator* OccupyIndicator = Cast<UCCOccupyIndicator>(GetWidget());
		if (OccupyIndicator)
		{
			OccupyIndicator->Init(Tree);
		}
	}
}
	