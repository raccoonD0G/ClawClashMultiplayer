// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/Components/AIActorComponent.h"
#include "PlayerDetectAIComponent.generated.h"

class UBlackboardComponent;
class ACCPaperPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASHMULTIPLAYER_API UPlayerDetectAIComponent : public UAIActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerDetectAIComponent();

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandlePlayerFirstDetected(ACCPaperPlayer* DetectedPlayer);

	UFUNCTION()
	void HandleNoPlayerDetected();
};
