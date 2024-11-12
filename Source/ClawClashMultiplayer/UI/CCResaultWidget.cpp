// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCResaultWidget.h"
#include "Components/TextBlock.h"
#include "ClawClashMultiplayer/GameInstance/CCGameInstance.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UCCResaultWidget::Init(EPlayerTeam InWinnerTeam)
{
	WinnerTeam = InWinnerTeam;

	if (WinnerTeam == EPlayerTeam::Red)
	{
		GameResaultText->SetText(FText::FromString(*FString::Printf(TEXT("Game Resault : Red Win!"))));
	}
	else if (WinnerTeam == EPlayerTeam::Blue)
	{
		GameResaultText->SetText(FText::FromString(*FString::Printf(TEXT("Game Resault : Blue Win!"))));
	}
	else
	{
		GameResaultText->SetText(FText::FromString(*FString::Printf(TEXT("Game Resault : Draw"))));
	}
}

void UCCResaultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuitButton->OnClicked.AddDynamic(this, &UCCResaultWidget::OnQuitButtonClicked);
}

void UCCResaultWidget::OnQuitButtonClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	}
}
