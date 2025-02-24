// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCGameLobby.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/GameState/CCLobbyGameState.h"
#include "Components/TextBlock.h"
#include <ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h>
#include "Components/Button.h"
#include <ClawClashMultiplayer/Character/Player/CCLobbyPlayerController.h>

void UCCGameLobby::NativeConstruct()
{
	Super::NativeConstruct();

	EntryGameState = GetWorld()->GetGameState<ACCLobbyGameState>();
	EntryGameState->OnPlayerCountChangedEvent.AddDynamic(this, &UCCGameLobby::OnPlayerCountChange);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCCGameLobby::SetTeamText, 0.1f, false);

	EntryGameState->OnReadyChangedEvent.AddDynamic(this, &UCCGameLobby::OnReadyChange);
	ReadyButton->OnClicked.AddDynamic(this, &UCCGameLobby::SetReady);

}

void UCCGameLobby::SetReady()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	ACCLobbyPlayerController* PlayerController = Cast<ACCLobbyPlayerController>(Controller);

	if (PlayerController)
	{
		PlayerController->Server_OnReadyButtonClicked();
	}
}

void UCCGameLobby::OnPlayerCountChange(int32 PlayerCount)
{
	PlayerCountText->SetText(FText::FromString(*FString::Printf(TEXT("Current Player : %d"), PlayerCount)));

	if (PlayerCount == 2)
	{
		ExplainText->SetText(FText::FromString(*FString::Printf(TEXT("Set \"Ready\" To Start Game"))));
	}
}

void UCCGameLobby::OnReadyChange(bool bIsRedPlayerReady, bool bIsBluePlayerReady)
{
	if (bIsRedPlayerReady)
	{
		RedPlayerReadyText->SetText(FText::FromString(*FString::Printf(TEXT("Red Player Ready : O"))));
	}
	else
	{
		RedPlayerReadyText->SetText(FText::FromString(*FString::Printf(TEXT("Red Player Ready : X"))));
	}

	if (bIsBluePlayerReady)
	{
		BluePlayerReadyText->SetText(FText::FromString(*FString::Printf(TEXT("Blue Player Ready : O"))));
	}
	else
	{
		BluePlayerReadyText->SetText(FText::FromString(*FString::Printf(TEXT("Blue Player Ready : X"))));
	}

	if (bIsRedPlayerReady && bIsBluePlayerReady)
	{
		ExplainText->SetText(FText::FromString(*FString::Printf(TEXT("Game Will Start Soon..."))));
	}
}

void UCCGameLobby::SetTeamText()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	ACCTeamPlayerState* PlayerState = (Controller->GetPlayerState<ACCTeamPlayerState>());
	if (PlayerState)
	{
		if (PlayerState->GetTeam() == EPlayerTeam::Blue)
		{
			TeamText->SetText(FText::FromString(*FString::Printf(TEXT("You are Blue Team"))));
		}
		else
		{
			TeamText->SetText(FText::FromString(*FString::Printf(TEXT("You are Red Team"))));
		}
	}
}