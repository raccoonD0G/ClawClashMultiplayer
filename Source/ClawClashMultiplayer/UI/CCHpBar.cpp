// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/UI/CCHpBar.h"
#include "ClawClashMultiplayer/Components/HealthComponent.h"
#include "Components/ProgressBar.h"

void UCCHpBar::Init(UHealthComponent* InHealthComponent)
{
	HealthComponent = InHealthComponent;
	HpBar->SetPercent(HealthComponent->GetHealthRatio());
	HealthComponent->OnHealthChanged.AddDynamic(this, &UCCHpBar::UpdateHpBar);
}

void UCCHpBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCCHpBar::UpdateHpBar(int32 NewCurrentHealth, int32 MaxHp)
{
	HpBar->SetPercent(HealthComponent->GetHealthRatio());
}
