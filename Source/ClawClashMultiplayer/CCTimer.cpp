// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTimer.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/UI/CCBattleWidget.h"
#include "Managers/UIManager/CCUIManager.h"

// Sets default values
ACCTimer::ACCTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    TotalGameTime = 180;
    bAlwaysRelevant = true;
}

void ACCTimer::Init(UCCBattleWidget* NewBattleWidget)
{
    BattleWidget = NewBattleWidget;
}

// Called when the game starts or when spawned
void ACCTimer::BeginPlay()
{
	Super::BeginPlay();

    if (HasAuthority())
    {
        RemainingGameTime = TotalGameTime;
        GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ACCTimer::UpdateGameTimer, 1.0f, true);
    }

    if (!HasAuthority())
    {
        if (UCCLevelWidget* LevelWidget = UCCUIManager::GetInstance()->GetLevelWidget())
        {
            BattleWidget = Cast<UCCBattleWidget>(LevelWidget);
        }
    }
}

void ACCTimer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACCTimer, RemainingGameTime);
}

void ACCTimer::OnRep_RemainingGameTime()
{
    if (BattleWidget)
    {
        BattleWidget->SetTimeText(RemainingGameTime);
    }
}

void ACCTimer::UpdateGameTimer()
{
    RemainingGameTime--;

    if (RemainingGameTime <= 0)
    {
        RemainingGameTime = TotalGameTime;
        GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
        OnTimerEndEvent.Broadcast();
    }
}