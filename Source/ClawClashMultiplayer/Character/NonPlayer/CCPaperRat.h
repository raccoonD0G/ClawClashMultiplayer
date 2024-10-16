// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCPaperNonPlayer.h"
#include "ClawClashMultiplayer/Interfaces/CCMoveable.h"
#include "CCPaperRat.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPaperRat : public ACCPaperNonPlayer
{
	GENERATED_BODY()
public:
	ACCPaperRat();

protected:
	virtual void BeginPlay() override;
	
// Exp Section
protected:
	int32 ExpAmount;

// Move Section
public:
	virtual void StartMove() override;
	virtual void EndMove() override;

// Collision Section
protected:
	UPROPERTY()
	TObjectPtr<USphereComponent> SphereComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
