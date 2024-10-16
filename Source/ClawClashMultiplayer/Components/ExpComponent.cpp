// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UExpComponent::UExpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	MaxExp = 1000;
	Exp = 0;
	MaxExpMultiplier = 1.2f;

	Level = 1;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UExpComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UExpComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UExpComponent, MaxExp);
	DOREPLIFETIME(UExpComponent, Exp);
	DOREPLIFETIME(UExpComponent, Level);
}

void UExpComponent::AddExp(int32 InExp)
{
	float ExpMultiplier = 1.0f;

	for (TFunctionRef<float()> Func : ExpMultipliers)
	{
		ExpMultiplier *= Func();
	}

	Exp += InExp * ExpMultiplier;
	if (Exp > MaxExp)
	{
		LevelUp();
	}
	OnExpChange.Broadcast(Exp);
}

void UExpComponent::LevelUp()
{
	Exp -= MaxExp;
	MaxExp *= MaxExpMultiplier;
	Level++;
	OnLevelChange.Broadcast(Level);
	OnExpChange.Broadcast(Exp);
}

void UExpComponent::OnRep_MaxExp()
{
}

void UExpComponent::OnRep_Exp()
{
	OnExpChange.Broadcast(Exp);
}

void UExpComponent::OnRep_Level()
{
	OnLevelChange.Broadcast(Level);
}