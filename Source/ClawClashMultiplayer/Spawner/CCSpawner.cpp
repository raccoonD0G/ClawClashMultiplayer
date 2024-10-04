// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawner.h"
#include "ClawClashMultiplayer/Character/NonPlayer/CCPaperNonPlayer.h"
#include "ClawClashMultiplayer/Managers/LayerManager/CCLayerManager.h"

void ACCSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACCSpawner::SpawnCharacter, SpawnInterval, true, SpawnInterval);
}

void ACCSpawner::Init(FSpawnableField SpawnableField)
{
	LeftEnd = SpawnableField.LeftEnd;
	RightEnd = SpawnableField.RightEnd;
	MaxCharacterNum = SpawnableField.MaxCharacterNum;
}

void ACCSpawner::SpawnCharacter()
{
	if (!HasAuthority())
	{
		return;
	}

	if (MaxCharacterNum > Charaters.Num())
	{
		float RandomX = FMath::FRandRange(LeftEnd.X, RightEnd.X);
		FVector SpawnPos(RandomX, UCCLayerManager::GetNonPlayerY(), LeftEnd.Z);

		Mutex.Lock();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACCPaperNonPlayer* SpawnedCharacter = GetWorld()->SpawnActor<ACCPaperNonPlayer>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParameters);
		if (SpawnedCharacter)
		{
			SpawnedCharacter->OnNonPlayerCharacterDestroyed.AddDynamic(this, &ACCSpawner::OnBeginDestroy);
			SpawnedCharacter->Init(LeftEnd.X, RightEnd.X);
			Charaters.Add(SpawnedCharacter);
		}
		Mutex.Unlock();
	}

	if (Charaters.Num() >= MaxCharacterNum)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void ACCSpawner::OnBeginDestroy(ACCPaperNonPlayer* DestroyedCharacter)
{
	if (!HasAuthority())
	{
		return;
	}

	Charaters.Remove(DestroyedCharacter);

	if ((!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle)) && Charaters.Num() < MaxCharacterNum)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACCSpawner::SpawnCharacter, SpawnInterval, true, SpawnInterval);
	}
}
