// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ClawClashMultiplayer/StageMap/CCStageMapDef.h"
#include "CCFieldCollider.generated.h"
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCFieldCollider : public UBoxComponent
{
	GENERATED_BODY()

public:
    UCCFieldCollider();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void Init(EFieldType NewFieldType, FVector NewFieldLocation, FVector NewFieldExtent);
	
protected:
    UPROPERTY(ReplicatedUsing = OnRep_FieldRelativeLocation)
    FVector FieldRelativeLocation;

    UPROPERTY(ReplicatedUsing = OnRep_FieldExtent)
    FVector FieldExtent;

    UFUNCTION()
    void OnRep_FieldRelativeLocation();

    UFUNCTION()
    void OnRep_FieldExtent();

    EFieldType FieldType;
};
