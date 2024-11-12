// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CCGameInstance.generated.h"


class UCCStageMapManager;
enum class EPlayerTeam : uint8;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UCCGameInstance();

    virtual void Init() override;
    virtual void OnStart() override;
    virtual void Shutdown() override;

// Player Section
protected:
    UPROPERTY()
    TMap<EPlayerTeam, FString> TeamIDMap;

public:
    void AddID(EPlayerTeam Team, FString ID);
    void RemoveID(EPlayerTeam Team);

    FORCEINLINE FString GetTeamPlayerID(EPlayerTeam Team) { return *TeamIDMap.Find(Team); }

// Resault Section
public:
    FORCEINLINE void SetWinnerTeam(EPlayerTeam InWinnerTeam) { WinnerTeam = InWinnerTeam; }
    FORCEINLINE EPlayerTeam GetWinnerTeam() { return WinnerTeam; }

    FORCEINLINE void SetBlueScore(int32 InBlueScore) { BlueScore = InBlueScore; }
    FORCEINLINE int32 GetBlueScore() { return BlueScore; }

    FORCEINLINE void SetRedScore(int32 InRedScore) { RedScore = InRedScore; }
    FORCEINLINE int32 GetRedScore() { return RedScore; }

protected:
    EPlayerTeam WinnerTeam;
    int32 BlueScore;
    int32 RedScore;
};
