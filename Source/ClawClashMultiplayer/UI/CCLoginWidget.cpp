// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCLoginWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include <ClawClashMultiplayer/GameMode/CCLobbyGameMode.h>
#include "ClawClashMultiplayer/CCHttpLogin.h"
#include <ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h>

void UCCLoginWidget::Init(ACCHttpLogin* InHttpLogin)
{
    HttpLogin = InHttpLogin;
}

void UCCLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (LoginButton)
    {
        LoginButton->OnClicked.AddDynamic(this, &UCCLoginWidget::OnLoginButtonClicked);
    }
}

void UCCLoginWidget::OnLoginButtonClicked()
{

    if (IDTextBox && PWTextBox)
    {
        FString ID = IDTextBox->GetText().ToString();
        FString PW = PWTextBox->GetText().ToString();

        UE_LOG(LogTemp, Log, TEXT("ID: %s, PW: %s"), *ID, *PW);

        EPlayerTeam Team = EPlayerTeam::None;

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        ACCTeamPlayerState* TeamPlayerState = Cast<ACCTeamPlayerState>(PlayerController->PlayerState);
        if (TeamPlayerState)
        {
            Team = TeamPlayerState->GetTeam();
        }
        
        if (HttpLogin && Team != EPlayerTeam::None)
        {
            HttpLogin->Server_SendLoginRequest(ID, PW, Team);
        }
    }
}