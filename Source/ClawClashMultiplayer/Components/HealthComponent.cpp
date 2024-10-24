// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/Interfaces/CCRespawnable.h"
#include <ClawClashMultiplayer/Interfaces/CCDestroyable.h>

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHp = 100;
	CurrentHp = 100;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, MaxHp);
	DOREPLIFETIME(UHealthComponent, CurrentHp);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHp = MaxHp;
}

void UHealthComponent::OnRep_CurrentHp()
{
	OnHealthChanged.Broadcast(CurrentHp, MaxHp);
}

void UHealthComponent::OnRep_MaxHp()
{
	OnHealthChanged.Broadcast(CurrentHp, MaxHp);
}

void UHealthComponent::Init(int32 NewMaxHp)
{
	MaxHp = NewMaxHp;
	CurrentHp = MaxHp;
}

void UHealthComponent::GetDamaged(int32 Amount, AActor* DamageCause)
{
	CurrentHp -= Amount;
	
	if (CurrentHp <= 0)
	{
		TScriptInterface<ICCRespawnable> Respawnable = TScriptInterface<ICCRespawnable>(GetOwner());

		if (Respawnable)
		{
			Respawnable->OnDeath();
		}
		else
		{
			TScriptInterface<ICCDestroyable> Destroyable = TScriptInterface<ICCDestroyable>(GetOwner());
			if (Destroyable)
			{
				Destroyable->OnDeath(DamageCause);
			}
		}
	}

	OnHealthChanged.Broadcast(CurrentHp, MaxHp);
}

void UHealthComponent::GetHeal(int32 Amount)
{
	CurrentHp += Amount;
	if (CurrentHp > MaxHp)
	{
		CurrentHp = MaxHp;
	}

	OnHealthChanged.Broadcast(CurrentHp, MaxHp);
}

