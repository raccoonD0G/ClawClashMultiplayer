// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCLayerManager.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCLayerManager : public UObject
{
	GENERATED_BODY()
	
// Layer Section
public:
    static float GetBackgroundY();
    static float GetBeforePlayerSpriteY();
    static float GetFieldTileY();
    static float GetNonPlayerY();
    static float GetTreeY();
    static float GetPlayerY();
    static float GetAfterPlayerSpriteY();
};
