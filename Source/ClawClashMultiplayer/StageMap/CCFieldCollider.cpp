// Fill out your copyright notice in the Description page of Project Settings.


#include "CCFieldCollider.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/StageMap/CCTileMapActor.h"
#include "ClawClashMultiplayer/Managers/CCGameManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "Net/UnrealNetwork.h"


UCCFieldCollider::UCCFieldCollider()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetCollisionProfileName(TEXT("CCGround"));
    SetIsReplicatedByDefault(true);
}

void UCCFieldCollider::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCCFieldCollider, FieldRelativeLocation);
    DOREPLIFETIME(UCCFieldCollider, FieldExtent);
}

void UCCFieldCollider::Init(EFieldType NewFieldType, FVector NewFieldRelativeLocation, FVector NewFieldExtent)
{
    FieldType = NewFieldType;
    FieldRelativeLocation = NewFieldRelativeLocation;
    FieldExtent = NewFieldExtent;

    SetRelativeLocation(NewFieldRelativeLocation);
    SetBoxExtent(NewFieldExtent);
}

void UCCFieldCollider::OnRep_FieldRelativeLocation()
{
    SetRelativeLocation(FieldRelativeLocation);
}

void UCCFieldCollider::OnRep_FieldExtent()
{
    SetBoxExtent(FieldExtent);
}
