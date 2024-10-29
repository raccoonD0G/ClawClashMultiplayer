// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameMode/CCLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "ClawClashMultiplayer/GameState/CCLobbyGameState.h"

ACCLobbyGameMode::ACCLobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ACCLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		if (RedPlayerController == nullptr)
		{
			RedPlayerController = NewPlayer;
			ACCTeamPlayerState* PlayerState = Cast<ACCTeamPlayerState>(RedPlayerController->PlayerState);
			PlayerState->SetTeam(EPlayerTeam::Red);

			ACCLobbyGameState* EntryGameState = GetGameState<ACCLobbyGameState>();
			if (EntryGameState)
			{
				EntryGameState->AddPlayerCount();
			}
		}
		else
		{
			BluePlayerController = NewPlayer;
			ACCTeamPlayerState* PlayerState = Cast<ACCTeamPlayerState>(BluePlayerController->PlayerState);
			PlayerState->SetTeam(EPlayerTeam::Blue);

			ACCLobbyGameState* EntryGameState = GetGameState<ACCLobbyGameState>();
			if (EntryGameState)
			{
				EntryGameState->AddPlayerCount();
			}
		}
	}
}

void ACCLobbyGameMode::SetRedPlayerReady()
{
	bIsRedPlayerReady = !bIsRedPlayerReady;
	ACCLobbyGameState* EntryGameState = GetGameState<ACCLobbyGameState>();
	if (EntryGameState)
	{
		EntryGameState->SetRedPlayerReady(bIsRedPlayerReady);
	}

	if (bIsBluePlayerReady && bIsRedPlayerReady)
	{
		StartMatchInSec(5.0f);
	}
}

void ACCLobbyGameMode::SetBluePlayerReady()
{
	bIsBluePlayerReady = !bIsBluePlayerReady;
	ACCLobbyGameState* EntryGameState = GetGameState<ACCLobbyGameState>();
	if (EntryGameState)
	{
		EntryGameState->SetBluePlayerReady(bIsBluePlayerReady);
	}

	if (bIsBluePlayerReady && bIsRedPlayerReady)
	{
		StartMatchInSec(5.0f);
	}
}

void ACCLobbyGameMode::StartMatchInSec(float Sec)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(Handle))
	{
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACCLobbyGameMode::StartMatch, Sec, false, Sec);
	}
}

void ACCLobbyGameMode::StartMatch()
{
	if (HasAuthority())
	{
		FString LevelName = TEXT("/Game/Maps/BattleLevel");
		GetWorld()->ServerTravel(LevelName, true);
	}
}