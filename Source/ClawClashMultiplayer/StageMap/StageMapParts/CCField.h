// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClashMultiplayer/StageMap/CCStageMapDef.h"
#include "CCTileMapLineParts.h"
#include "CCField.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCField : public UCCTileMapLineParts
{
	GENERATED_BODY()
	
public:
	UCCField();

protected:
	EFieldType FieldType;

public:
	void AddStartPos(int32 AddedNum);
	EFieldType GetFieldType();
	void Init(FIntVector2 NewTileMapPos, int32 NewLength, EFieldType NewFieldTypee);
};
