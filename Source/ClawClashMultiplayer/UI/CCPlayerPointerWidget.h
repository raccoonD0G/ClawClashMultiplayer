// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCWorldSpaceWidget.h"
#include <Components/Image.h>
#include "CCPlayerPointerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCPlayerPointerWidget : public UCCWorldSpaceWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetColor(FLinearColor InColor) { PlayerPointer->SetColorAndOpacity(InColor); }

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerPointer;
};
