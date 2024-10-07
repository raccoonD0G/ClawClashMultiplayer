// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CCTeamPlayerState.generated.h"

UENUM()
enum class PlayerTeam : uint8
{
	Red,
	Blue
};

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCTeamPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACCTeamPlayerState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

// Team Section
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Team, VisibleAnywhere, BlueprintReadOnly)
	PlayerTeam Team;

	UFUNCTION()
	void OnRep_Team();

public:
	FORCEINLINE void SetTeam(PlayerTeam InTeam) { Team = InTeam; }
	FORCEINLINE PlayerTeam GetTeam() { return Team; }

};
