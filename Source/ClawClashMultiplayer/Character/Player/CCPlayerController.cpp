// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerController.h"
#include "CCPaperPlayer.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "ClawClashMultiplayer/UI/CCBattleWidget.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/CCPlayerSpawner.h"

ACCPlayerController::ACCPlayerController()
{
	bReplicates = true;
}

void ACCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void ACCPlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);
}

void ACCPlayerController::PostSeamlessTravel()
{
    Super::PostSeamlessTravel();
}

void ACCPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();

    if (!GetPawn())
    {
        return;
    }

    if (IsLocalController())
    {
        if (BattleWidgetClass)
        {
            BattleWidget = CreateWidget<UCCBattleWidget>(GetWorld(), BattleWidgetClass);

            if (BattleWidget)
            {
                
                if (GetPawn()->FindComponentByClass<UExpComponent>())
                {
                    BattleWidget->Init(GetPawn()->FindComponentByClass<UExpComponent>());
                }

                BattleWidget->AddToViewport();
            }
        }
    }
}
