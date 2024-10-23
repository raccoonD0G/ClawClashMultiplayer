// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCGameLobby.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/GameState/CCEntryGameState.h"
#include "Components/TextBlock.h"

void UCCGameLobby::NativeConstruct()
{
	Super::NativeConstruct();

	EntryGameState = GetWorld()->GetGameState<ACCEntryGameState>();
	EntryGameState->OnPlayerCountChangedEvent.AddDynamic(this, &UCCGameLobby::OnPlayerCountChange);
}

void UCCGameLobby::OnPlayerCountChange(int32 PlayerCount)
{
	PlayerCountText->SetText(FText::FromString(*FString::Printf(TEXT("Current Player : %d"), PlayerCount)));

	if (PlayerCount == 2)
	{
		ExplainText->SetText(FText::FromString(*FString::Printf(TEXT("Game Will Start Soon..."))));
	}
}
