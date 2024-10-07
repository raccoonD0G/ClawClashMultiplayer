// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "ClawClashMultiplayer/StageMap/CCStageMapDef.h"
#include "ClawClashMultiplayer/Managers/SpawnManager/CCSpawn.h"
#include "CCTileMapActor.generated.h"

class UPaperTileMap;
class UCCField;
class UCCPlatform;
class UCCRoom;
class UPaperSpriteComponent;
class UCCBoxQuadTreeNode;
class UCCFieldCollider;
class UPaperTileMapComponent;
struct FCCFeatureInfoArrContainer;
class UPaperSprite;
struct FSpawnableField;
class ACCSpawnerSpawner;
class UCCFieldTrigger;
class ACCTree;

USTRUCT()
struct FPlatformEdge
{
    GENERATED_BODY()
public:

    int32 PlatformIndex1;
    int32 PlatformIndex2;
    float Weight;
    FIntVector2 Pos1;
    FIntVector2 Pos2;

    FPlatformEdge()
    {

    }

    FPlatformEdge(int32 InPlatformIndex1, int32 InPlatformIndex2, float InWeight, FIntVector2 InPos1, FIntVector2 InPos2)
        : PlatformIndex1(InPlatformIndex1), PlatformIndex2(InPlatformIndex2), Weight(InWeight), Pos1(InPos1), Pos2(InPos2)
    {
    }

    bool operator<(const FPlatformEdge& Other) const
    {
        return Weight < Other.Weight;
    }
};

USTRUCT()
struct FCCFieldStruct
{
    GENERATED_BODY()

public:
    UPROPERTY()
    int32 TileMapPosX;

    UPROPERTY()
    int32 TileMapPosY;

    UPROPERTY()
    int32 Length;

    UPROPERTY()
    EFieldType FieldType;

    FCCFieldStruct()
        : TileMapPosX(0), TileMapPosY(0), Length(0), FieldType(EFieldType::BasicField) {}

    FCCFieldStruct(FIntVector2 NewTileMapPos, int32 NewLength, EFieldType NewFieldType)
        : TileMapPosX(NewTileMapPos.X), TileMapPosY(NewTileMapPos.Y), Length(NewLength), FieldType(NewFieldType) {}

    FCCFieldStruct(const FCCFieldStruct& Field)
        : TileMapPosX(Field.TileMapPosX), TileMapPosY(Field.TileMapPosY), Length(Field.Length), FieldType(Field.FieldType) {}

    FCCFieldStruct& operator=(const FCCFieldStruct& Field)
    {
        if (this != &Field)
        {
            TileMapPosX = Field.TileMapPosX;
            TileMapPosY = Field.TileMapPosY;
            Length = Field.Length;
            FieldType = Field.FieldType;
        }
        return *this;
    }

    bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
    {
        Ar << TileMapPosX;
        Ar << TileMapPosY;
        Ar << Length;
        Ar << FieldType;

        bOutSuccess = true;
        return true;
    }
};

USTRUCT()
struct FSpriteComponentInfo
{
    GENERATED_BODY()

public:
    UPROPERTY()
    EFeatureType FeatureType;

    UPROPERTY()
    int32 SpriteIndex;

    UPROPERTY()
    FVector RelativePos;

    UPROPERTY()
    float RandomScale;

    UPROPERTY()
    bool bIsBeforePlayer;

    FSpriteComponentInfo()
        : FeatureType(EFeatureType::WeedFeature), SpriteIndex(0), RelativePos(FVector::Zero()), RandomScale(1.0f), bIsBeforePlayer(true) {}

    FSpriteComponentInfo(EFeatureType InFeatureType, int32 InSpriteIndex, FVector InRelativePos, float InRandomScale, bool InbIsBeforePlayer)
        : FeatureType(InFeatureType), SpriteIndex(InSpriteIndex), RelativePos(InRelativePos), RandomScale(InRandomScale), bIsBeforePlayer(InbIsBeforePlayer) {}

    bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
    {
        Ar << FeatureType;
        Ar << SpriteIndex;
        Ar << RelativePos;
        Ar << RandomScale;
        Ar << bIsBeforePlayer;

        bOutSuccess = true;
        return true;
    }
};

template<>
struct TStructOpsTypeTraits<FSpriteComponentInfo> : public TStructOpsTypeTraitsBase2<FSpriteComponentInfo>
{
    enum
    {
        WithNetSerializer = true,
    };
};


template<>
struct TStructOpsTypeTraits<FCCFieldStruct> : public TStructOpsTypeTraitsBase2<FCCFieldStruct>
{
    enum
    {
        WithNetSerializer = true,
    };
};


UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCTileMapActor : public AActor
{
    GENERATED_BODY()

public:
    ACCTileMapActor();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Info Section
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileMapWidth = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileMapHeight = 64;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileWidth = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileHeight = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 MinRoomHeight = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 MinRoomWidth = 11;

public:
    FORCEINLINE int32 GetTileMapWidth() const { return TileMapWidth; }
    FORCEINLINE int32 GetTileMapHeight() const { return TileMapHeight; }
    FORCEINLINE int32 GetTileWidth() const { return TileWidth; }
    FORCEINLINE int32 GetTileHeight() const { return TileHeight; }
    FORCEINLINE int32 GetMinRoomHeight() const { return MinRoomHeight; }
    FORCEINLINE int32 GetMinRoomWidth() const { return MinRoomWidth; }

// FieldTile Section
protected:
    UPROPERTY()
    TObjectPtr<UPaperTileMapComponent> FieldTileMapComponent;

    UPROPERTY()
    TObjectPtr<UPaperTileSet> FieldTileSet;

// Create Map Section
public:
    FORCEINLINE const TArray<UCCField*>& GetFieldArr() const { return FieldArr; }

protected:
    UPROPERTY()
    TArray<TObjectPtr<UCCField>> FieldArr;

    UPROPERTY(ReplicatedUsing = OnRep_FieldStructArr)
    TArray<FCCFieldStruct> FieldStructArr;

    UFUNCTION()
    void OnRep_FieldStructArr();

    FVector GetWorldSpaceStartPos(UCCField* Field) const;
    FVector GetWorldSpaceEndPos(UCCField* Field) const;

    void SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth) const;
    void GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight) const;
    float CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2) const;
    void GenerateMST(TArray<UCCPlatform*>& PlatformArr) const;
    void CreatePlatformsAlongEdge(TArray<UCCPlatform*>& PlatformArr, const FPlatformEdge& Edge) const;
    void InitializeTileMap(UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight);

// Tile Section
public:
    bool SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly = true);
    void CreatFieldTile(UCCField* Field);

// Collision Section
public:
    void SetupTileColliders(int32 Column, int32 Row, int32 ColliderLength, EFieldType NewFieldType);

    UPROPERTY(ReplicatedUsing = OnRep_FieldColliderArr)
    TArray<TObjectPtr<UCCFieldCollider>> FieldColliderArr;

    UFUNCTION()
    void OnRep_FieldColliderArr();

    UPROPERTY(ReplicatedUsing = OnRep_FieldTriggerArr)
    TArray<TObjectPtr<UCCFieldTrigger>> FieldTriggerArr;

    UFUNCTION()
    void OnRep_FieldTriggerArr();

// Sprite Section
public:

    FORCEINLINE int32 GetBeforePlayerOrder() const { return BeforePlayerOrder; }
    FORCEINLINE int32 GetAfterPlayerOrder() const { return AfterPlayerOrder; }

    FORCEINLINE void AddBeforPlayerOrder() { BeforePlayerOrder++; }
    FORCEINLINE void AddAfterPlayerOrder() { AfterPlayerOrder++; }

    void PlaceSpriteEachField(UCCField* Field);
    void PlaceSprites(UCCField* Field, float TileInterval, EFeatureType FeatureType, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree = true, int32 MinSpriteNum = 0, int32 MaxSpriteNum = 100);
    
    FBox2D GetSpriteBox(UPaperSprite* PaperSprite, float Scale, FVector MiddleButtonPos);
    void CreateAndAttachSpriteComponent(UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale, bool bIsBeforePlayer);
    FVector CalculateSpriteWorldSpacePos(float XPos, FVector StartPos, UPaperSprite* FeatureSprite, bool bIsBeforePlayer);

protected:
    UPROPERTY()
    TObjectPtr<UCCBoxQuadTreeNode> RootNode;

    UPROPERTY(ReplicatedUsing = OnRep_SpriteComponentInfoArr)
    TArray<FSpriteComponentInfo> SpriteComponentInfoArr;

    UFUNCTION()
    void OnRep_SpriteComponentInfoArr();

    int32 BeforePlayerOrder;
    int32 AfterPlayerOrder;

// Spawn Section
protected:
    FSpawnableField ChangeIntoSpawnableField(UCCField* Field, ESpawnableType SpawnableType, int32 MaxCharacterNum);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<ACCSpawnerSpawner> SpawnerSpawner;

// Tree Section
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
    TSubclassOf<ACCTree> TreeClass;

    UPROPERTY(ReplicatedUsing = OnRep_Trees)
    TArray<TObjectPtr<ACCTree>> Trees;

    UFUNCTION()
    void OnRep_Trees();
};
