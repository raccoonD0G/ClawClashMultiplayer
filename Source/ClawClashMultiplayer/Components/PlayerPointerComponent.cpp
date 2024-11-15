// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/PlayerPointerComponent.h"
#include <ClawClashMultiplayer/Character/Player/CCPaperPlayer.h>
#include <ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h>
#include <ClawClashMultiplayer/UI/CCPlayerPointerWidget.h>

void UPlayerPointerComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(InitHandle, this, &UPlayerPointerComponent::SetPlayerPointerColor, 1.0f, false);
}

void UPlayerPointerComponent::SetPlayerPointerColor()
{
	ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(GetOwner());
	if (Player)
	{
		UCCPlayerPointerWidget* PlayerPointerWidget = Cast<UCCPlayerPointerWidget>(GetUserWidgetObject());
		if (!PlayerPointerWidget)
		{
			return;
		}

		ACCTeamPlayerState* PlayerState = Cast<ACCTeamPlayerState>(Player->GetPlayerState());
		if (!PlayerState)
		{
			GetWorld()->GetTimerManager().SetTimer(InitHandle, this, &UPlayerPointerComponent::SetPlayerPointerColor, 1.0f, false);
			return;
		}

		if (PlayerState)
		{
			if (PlayerState->GetTeam() == EPlayerTeam::Blue)
			{
				PlayerPointerWidget->SetColor(FLinearColor::Blue);
			}
			else if (PlayerState->GetTeam() == EPlayerTeam::Red)
			{
				PlayerPointerWidget->SetColor(FLinearColor::Red);
			}
		}
	}
}
