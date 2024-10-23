// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCOnDeathWidget.h"
#include "Components/TextBlock.h"

UCCOnDeathWidget::UCCOnDeathWidget()
{
}

void UCCOnDeathWidget::Countdown()
{
	if (TimeLeft >= 1)
	{
		TimeLeft--;
	}
	RespawnText->SetText(FText::FromString(*FString::Printf(TEXT("Respawning in %d seconds"), TimeLeft)));
}

void UCCOnDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TimeLeft = 5;
	FTimerHandle CountdownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UCCOnDeathWidget::Countdown, 1.0f, true);
}
