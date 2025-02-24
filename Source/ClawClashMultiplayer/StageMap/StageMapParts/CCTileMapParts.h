// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCTileMapParts.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCTileMapParts : public UObject
{
	GENERATED_BODY()
	
protected:
	FIntVector2 TileMapPos;

public:
	void SetTileMapPos(FIntVector2 NewTileMapPos);
	FIntVector2 GetTileMapPos() const;
	void Init(FIntVector2 NewTileMapPos);
	
};
