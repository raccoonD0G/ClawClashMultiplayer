// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClawClashMultiplayer/Spawner/CCSpawnerSpawner.h"
#include "CCSpawner.generated.h"

class ACCPaperNonPlayer;

UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCSpawner : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

// Mutex Section
protected:
	FCriticalSection Mutex;

protected:
	FVector LeftEnd;
	FVector RightEnd;
	int32 MaxCharacterNum;
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<ACCPaperNonPlayer> SpawnClass;

	UPROPERTY()
	TSet<TWeakObjectPtr<ACCPaperNonPlayer>> Charaters;

	FTimerHandle TimerHandle;

public:
	void Init(FSpawnableField SpawnableField);
	virtual void SpawnCharacter();

	UFUNCTION()
	void OnBeginDestroy(ACCPaperNonPlayer* DestroyedCharacter);
};
