// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Character/Player/CCLobbyPlayerController.h"
#include <ClawClashMultiplayer/Managers/UIManager/CCUIManager.h>
#include <Net/UnrealNetwork.h>
#include "ClawClashMultiplayer/UI/CCLoginWidget.h"
#include "ClawClashMultiplayer/CCHttpLogin.h"
#include <ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h>
#include <ClawClashMultiplayer/GameMode/CCLobbyGameMode.h>

void ACCLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	if (HasAuthority())
	{
		HttpLogin = GetWorld()->SpawnActor<ACCHttpLogin>();
		HttpLogin->SetOwner(this);
		HttpLogin->OnLoginSuccessEvent.AddDynamic(this, &ACCLobbyPlayerController::Client_CloseLoginWidget);
	}
}

void ACCLobbyPlayerController::Server_OnReadyButtonClicked_Implementation()
{
	if (GetPlayerState<ACCTeamPlayerState>()->GetTeam() == EPlayerTeam::Blue)
	{
		ACCLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ACCLobbyGameMode>();
		GameMode->SetBluePlayerReady();
	}
	else
	{
		ACCLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ACCLobbyGameMode>();
		GameMode->SetRedPlayerReady();
	}
}

void ACCLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCLobbyPlayerController, HttpLogin);
}


void ACCLobbyPlayerController::OnRep_HttpLogin()
{
	if (HttpLogin != nullptr)
	{
		if (LoginWidgetClass)
		{
			HttpLogin->SetOwner(this);
			LoginWidget = Cast<UCCLoginWidget>(UCCUIManager::GetInstance()->AddPopupWidget(GetWorld(), LoginWidgetClass));
			LoginWidget->Init(HttpLogin);
		}
	}
}

void ACCLobbyPlayerController::Client_CloseLoginWidget_Implementation()
{
	if (LoginWidget)
	{
		UCCUIManager::GetInstance()->RemoveTopPopupWidget(LoginWidget);
	}
}