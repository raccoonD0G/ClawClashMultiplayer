// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/HealthBarWidgetComponent.h"
#include "ClawClashMultiplayer/Components/HealthComponent.h"
#include "ClawClashMultiplayer/UI/CCHpBar.h"

UHealthBarWidgetComponent::UHealthBarWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetWidgetSpace(EWidgetSpace::World);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRelativeRotation(FRotator(0, 90, 0));
	SetRelativeLocation(FVector(0, 0, 500));
}

void UHealthBarWidgetComponent::InitWidget()
{
	Super::InitWidget();

	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();

	if (HealthComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UHpBarComponent need UHealthComponent"));
		return;
	}

	if (WidgetClass)
	{
		UCCHpBar* HpBarWidget = Cast<UCCHpBar>(GetUserWidgetObject());
		if (HpBarWidget)
		{
			HpBarWidget->Init(HealthComponent.Get());
		}
	}
}