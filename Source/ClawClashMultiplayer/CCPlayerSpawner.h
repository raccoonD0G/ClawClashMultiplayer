// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "CCPlayerSpawner.generated.h"

UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPlayerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCPlayerSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Team Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerTeam CurrentPlayerTeam;

public:
	FORCEINLINE EPlayerTeam GetPlayerTeam() { return CurrentPlayerTeam; }

};
