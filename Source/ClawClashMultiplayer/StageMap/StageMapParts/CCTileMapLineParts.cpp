// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapLineParts.h"
#include "CCTileMapParts.h"
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/StageMap/CCTileMapActor.h"
#include "CCField.h"
#include "CCPlatform.h"
#include "PaperTileMapComponent.h"
#include "PaperSpriteComponent.h"
#include "ClawClashMultiplayer/StageMap/CCBoxQuadTreeNode.h"
#include "ClawClashMultiplayer/Managers/LayerManager/CCLayerManager.h"

FIntVector2 UCCTileMapLineParts::GetStartPos() const
{
	return GetTileMapPos();
}

int32 UCCTileMapLineParts::GetLength() const
{
	return Length;
}

FIntVector2 UCCTileMapLineParts::GetEndPos() const
{
	return FIntVector2(GetTileMapPos().X + Length, GetTileMapPos().Y);
}