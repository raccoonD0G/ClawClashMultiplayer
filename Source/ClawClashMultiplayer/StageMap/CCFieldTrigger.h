// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CCFieldTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCFieldTrigger : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UCCFieldTrigger();
    virtual void BeginPlay();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void Init(FVector InTriggerRelativeLocation, FVector InTriggerExtent);

protected:
    UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
    UPROPERTY(ReplicatedUsing = OnRep_TriggerRelativeLocation)
    FVector TriggerRelativeLocation;

    UPROPERTY(ReplicatedUsing = OnRep_TriggerExtent)
    FVector TriggerExtent;

    UFUNCTION()
    void OnRep_TriggerRelativeLocation();

    UFUNCTION()
    void OnRep_TriggerExtent();
};
