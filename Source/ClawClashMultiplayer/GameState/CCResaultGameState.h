// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "CCResaultGameState.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCResaultGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACCResaultGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCCLevelWidget> GameResultWidget;

	UPROPERTY(ReplicatedUsing = OnRep_WinnerTeam)
	EPlayerTeam WinnerTeam;

	UFUNCTION()
	void OnRep_WinnerTeam();

	UPROPERTY()
	TObjectPtr<class UCCResaultWidget> ResaultWidget;

public:
	FORCEINLINE void SetWinnerTeam(EPlayerTeam InWinnerTeam) { WinnerTeam = InWinnerTeam; }
};
