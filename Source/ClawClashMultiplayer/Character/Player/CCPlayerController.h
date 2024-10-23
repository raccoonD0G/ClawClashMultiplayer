// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCPlayerController.generated.h"

class UUserWidget;
class UCCBattleWidget;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACCPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void PostSeamlessTravel() override;

	virtual void OnRep_Pawn() override;

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> BattleWidgetClass;

	UPROPERTY()
	TObjectPtr<UCCBattleWidget> BattleWidget;
};