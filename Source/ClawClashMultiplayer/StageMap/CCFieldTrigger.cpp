// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/StageMap/CCFieldTrigger.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "Components/CapsuleComponent.h"

UCCFieldTrigger::UCCFieldTrigger()
{
	SetCollisionProfileName("Trigger");
	SetIsReplicatedByDefault(true);
}

void UCCFieldTrigger::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UCCFieldTrigger::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UCCFieldTrigger::OnOverlapEnd);
}

void UCCFieldTrigger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCCFieldTrigger, TriggerRelativeLocation);
	DOREPLIFETIME(UCCFieldTrigger, TriggerExtent);
}

void UCCFieldTrigger::Init(FVector InTriggerRelativeLocation, FVector InTriggerExtent)
{
	TriggerExtent = InTriggerExtent;
	TriggerRelativeLocation = InTriggerRelativeLocation;
	OnRep_TriggerRelativeLocation();
	OnRep_TriggerExtent();
}

void UCCFieldTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
	if (Player)
	{
		Player->GetCapsuleComponent()->SetCollisionProfileName("CCPassThroughPlayer");
	}
}

void UCCFieldTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
	if (Player)
	{
		Player->GetCapsuleComponent()->SetCollisionProfileName("CCBlockedPlayer");
	}
}

void UCCFieldTrigger::OnRep_TriggerRelativeLocation()
{
	SetBoxExtent(TriggerExtent);
}

void UCCFieldTrigger::OnRep_TriggerExtent()
{
	SetRelativeLocation(TriggerRelativeLocation);
}
