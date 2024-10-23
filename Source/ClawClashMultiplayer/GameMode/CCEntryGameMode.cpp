// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameMode/CCEntryGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "ClawClashMultiplayer/GameState/CCEntryGameState.h"

ACCEntryGameMode::ACCEntryGameMode()
{
	bUseSeamlessTravel = true;
}

void ACCEntryGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		if (RedPlayerController == nullptr)
		{
			RedPlayerController = NewPlayer;
			ACCTeamPlayerState* PlayerState = Cast<ACCTeamPlayerState>(RedPlayerController->PlayerState);
			PlayerState->SetTeam(EPlayerTeam::Red);

			ACCEntryGameState* EntryGameState = GetGameState<ACCEntryGameState>();
			EntryGameState->AddPlayerCount();

		}
		else
		{
			BluePlayerController = NewPlayer;
			ACCTeamPlayerState* PlayerState = Cast<ACCTeamPlayerState>(BluePlayerController->PlayerState);
			PlayerState->SetTeam(EPlayerTeam::Blue);
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACCEntryGameMode::StartMatch, 5.0f, false, 5.0f);

			ACCEntryGameState* EntryGameState = GetGameState<ACCEntryGameState>();
			EntryGameState->AddPlayerCount();
		}
	}
}

void ACCEntryGameMode::StartMatch()
{
	if (HasAuthority())
	{
		FString LevelName = "/Game/Maps/BattleLevel";

		GetWorld()->ServerTravel(LevelName, true);
	}
}
