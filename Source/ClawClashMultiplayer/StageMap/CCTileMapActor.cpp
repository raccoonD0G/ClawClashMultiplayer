// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapActor.h"

#include "PaperTileMapComponent.h"
#include "PaperSprite.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/StageMap/StageMapParts/CCRoom.h"
#include "ClawClashMultiplayer/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClashMultiplayer/StageMap/StageMapParts/CCField.h"
#include "ClawClashMultiplayer/StageMap/CCStageMapDef.h"
#include "ClawClashMultiplayer/StageMap/CCUnionFind.h"
#include "ClawClashMultiplayer/StageMap/CCFieldCollider.h"
#include "ClawClashMultiplayer/StageMap/CCBoxQuadTreeNode.h"
#include "ClawClashMultiplayer/Managers/LayerManager/CCLayerManager.h"
#include "ClawClashMultiplayer/Spawner/CCSpawnerSpawner.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/StageMap/CCFieldTrigger.h"
#include "ClawClashMultiplayer/StageMap/CCTree.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/CCPlayerSpawner.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include <ClawClashMultiplayer/Interfaces/CCDestroyable.h>
#include "ClawClashMultiplayer/Components/DamageSphereComponent.h"

ACCTileMapActor::ACCTileMapActor()
{
	PrimaryActorTick.bCanEverTick = false;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;
    FieldTileMapComponent->SetCollisionProfileName(TEXT("NoCollision"));
    bReplicates = true;

    
}

void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        UE_LOG(LogTemp, Log, TEXT("TileMapBulidStart"));

        for (UCCField* Field : FieldArr)
        {
            CreatFieldTile(Field);
            PlaceSpriteEachField(Field);

            switch (Field->GetFieldType())
            {
            case EFieldType::DogHouseField:
                SpawnerSpawner->AddSpawnableField(ChangeIntoSpawnableField(Field, ESpawnableType::Dog, 1));
                break;
            case EFieldType::RaccoonHouseField:
                SpawnerSpawner->AddSpawnableField(ChangeIntoSpawnableField(Field, ESpawnableType::Raccoon, 1));
                break;
            case EFieldType::CaveField:
                SpawnerSpawner->AddSpawnableField(ChangeIntoSpawnableField(Field, ESpawnableType::Rat, 5));
                break;
            default:
                break;
            }
        }

        TSet<int32> TreeFieldIndexs;
        while (TreeFieldIndexs.Num() < 10)
        {
            TreeFieldIndexs.Add(FMath::RandRange(0, FieldArr.Num() - 1));
        }

        for (int32 Index : TreeFieldIndexs)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            float TreeX = FMath::RandRange(GetWorldSpaceStartPos(FieldArr[Index]).X, GetWorldSpaceEndPos(FieldArr[Index]).X);
            FVector TreePos(TreeX, UCCLayerManager::GetTreeY(), GetWorldSpaceStartPos(FieldArr[Index]).Z + GetTileHeight() / 2.0f);
            ACCTree* Tree = GetWorld()->SpawnActor<ACCTree>(TreeClass, TreePos, FRotator::ZeroRotator, SpawnParams);
            Trees.Add(Tree);
        }

        TArray<AActor*> PlayerSpawners;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCPlayerSpawner::StaticClass(), PlayerSpawners);

        for (AActor* Spawner : PlayerSpawners)
        {
            ACCPlayerSpawner* PlayerSpawner = Cast<ACCPlayerSpawner>(Spawner);

            if (PlayerSpawner)
            {
                UCCField* Field = FieldArr[FMath::RandRange(0, FieldArr.Num() - 1)];
                FVector StartPos = (GetWorldSpaceEndPos(Field) + GetWorldSpaceStartPos(Field)) / 2.0f;
                StartPos.Z += 1000.0f;
                if (PlayerSpawner->GetPlayerTeam() == EPlayerTeam::Blue)
                {
                    BluePlayerStartPos = StartPos;
                }
                else if (PlayerSpawner->GetPlayerTeam() == EPlayerTeam::Red)
                {
                    RedPlayerStartPos = StartPos;
                }
            }
        }

        OnRep_BluePlayerStartPos();
        OnRep_RedPlayerStartPos();

        UCCStageMapManager::GetInstance()->TurnTrueIsFieldGenerated();
        UCCStageMapManager::GetInstance()->TurnTrueIsTriggerGenerated();
        UCCStageMapManager::GetInstance()->TurnTrueIsColliderGenerated();
        UCCStageMapManager::GetInstance()->TurnTrueIsSpriteGenerated();

        UCCStageMapManager::GetInstance()->OnMapGenerated.Broadcast();

        if (MapBound)
        {
            MapBound->OnComponentEndOverlap.AddDynamic(this, &ACCTileMapActor::OnEndOverlap);
        }
    }
}

void ACCTileMapActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!UCCStageMapManager::GetInstance()->GetStageMap())
    {
        UCCStageMapManager::GetInstance()->SetStageMap(this);
    }

    if (HasAuthority())
    {
        if (FieldTileMapComponent && FieldTileSet)
        {
            FieldTileMapComponent->SetIsReplicated(true);
            UCCStageMapManager::GetInstance()->InitializeTileSet(FieldTileSet);
            InitializeTileMap(FieldTileSet, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
        }

        RootNode = NewObject<UCCBoxQuadTreeNode>();
        RootNode->Initialize(FVector2D(0, -GetTileMapHeight() * 512), FVector2D(GetTileMapWidth() * 512, 0), 10);

        TArray<UCCRoom*> RoomArr;
        TArray<UCCPlatform*> PlatformArr;
        GenerateRooms(RoomArr, TileMapWidth, TileMapHeight, MinRoomWidth, MinRoomHeight);
        for (UCCRoom* Room : RoomArr)
        {
            PlatformArr.Add(Room->GeneratePlatform());
        }
        GenerateMST(PlatformArr);

        for (UCCPlatform* Platform : PlatformArr)
        {
            for (UCCField* Field : Platform->GetFieldArr())
            {
                FieldArr.Add(Field);
                FieldStructArr.Add(FCCFieldStruct(Field->GetStartPos(), Field->GetLength(), Field->GetFieldType()));
            }
        }

        FieldTileMapComponent->RebuildCollision();

        MapBound = GetComponentByClass<UBoxComponent>();
        if (MapBound)
        {
            MapBound->SetRelativeLocation(FVector(TileMapWidth * TileWidth / 2.0f, 0.0f, -TileMapHeight * TileHeight / 2.0f));
            MapBound->SetBoxExtent(FVector(TileMapWidth * TileWidth / 1.8f, 100.0f, TileMapHeight * TileHeight / 1.8f));
        }
    }
}

void ACCTileMapActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACCTileMapActor, FieldStructArr);
    DOREPLIFETIME(ACCTileMapActor, SpriteComponentInfoArr);
    DOREPLIFETIME(ACCTileMapActor, FieldColliderArr);
    DOREPLIFETIME(ACCTileMapActor, FieldTriggerArr);
    DOREPLIFETIME(ACCTileMapActor, Trees);
    DOREPLIFETIME(ACCTileMapActor, RedPlayerStartPos);
    DOREPLIFETIME(ACCTileMapActor, BluePlayerStartPos);
    
}

void ACCTileMapActor::OnRep_FieldStructArr()
{
    if (FieldTileMapComponent && FieldTileSet)
    {
        UCCStageMapManager::GetInstance()->InitializeTileSet(FieldTileSet);
        InitializeTileMap(FieldTileSet, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
    }

    for (FCCFieldStruct Field : FieldStructArr)
    {
        UCCField* NewField = NewObject<UCCField>();
        NewField->Init(FIntVector2(Field.TileMapPosX, Field.TileMapPosY), Field.Length, Field.FieldType);
        FieldArr.Add(NewField);
    }
    
    for (UCCField* Field : FieldArr)
    {
        CreatFieldTile(Field);
    }

    FieldTileMapComponent->RebuildCollision();

    UCCStageMapManager::GetInstance()->TurnTrueIsFieldGenerated();
}

FVector ACCTileMapActor::GetWorldSpaceStartPos(UCCField* Field) const
{
    FVector WorldSpaceStartPos;
    WorldSpaceStartPos.X = Field->GetTileMapPos().X * GetTileWidth() + GetActorLocation().X;
    WorldSpaceStartPos.Y = GetActorLocation().Y;
    WorldSpaceStartPos.Z = -Field->GetTileMapPos().Y * GetTileHeight() + GetActorLocation().Z;
    return WorldSpaceStartPos;
}

FVector ACCTileMapActor::GetWorldSpaceEndPos(UCCField* Field) const
{
    int32 DecreasedLength = Field->GetLength() - 1;
    if (DecreasedLength * GetTileWidth() < GetTileWidth() * GetTileMapWidth() - GetWorldSpaceStartPos(Field).X)
        return GetWorldSpaceStartPos(Field) + FVector(DecreasedLength * GetTileWidth(), 0, 0);
    else
        return GetWorldSpaceStartPos(Field) + FVector(GetTileWidth() * (GetTileMapWidth() - 1) - GetWorldSpaceStartPos(Field).X, 0, 0);
}

void ACCTileMapActor::SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth) const
{
    if (Depth <= 0 || Space->Width <= MinWidth * 2 || Space->Height <= MinHeight * 2)
    {
        OutRooms.Add(Space);
        return;
    }

    bool SplitHorizontally = (Space->Width > Space->Height);
    if (SplitHorizontally)
    {
        int32 SplitPoint = FMath::RandRange(MinWidth, Space->Width - MinWidth);
        TObjectPtr<UCCRoom> Room1 = NewObject<UCCRoom>();
        Room1->Init(Space->GetTileMapPos(), SplitPoint, Space->Height, GetMinRoomWidth(), GetMinRoomHeight());
        TObjectPtr<UCCRoom> Room2 = NewObject<UCCRoom>();
        Room2->Init(FIntVector2(Space->GetTileMapPos().X + SplitPoint, Space->GetTileMapPos().Y), Space->Width - SplitPoint, Space->Height, GetMinRoomWidth(), GetMinRoomHeight());
        SplitSpace(OutRooms, Room1, MinWidth, MinHeight, Depth - 1);
        SplitSpace(OutRooms, Room2, MinWidth, MinHeight, Depth - 1);
    }
    else
    {
        int32 SplitPoint = FMath::RandRange(MinHeight, Space->Height - MinHeight);
        TObjectPtr<UCCRoom> Room1 = NewObject<UCCRoom>();
        Room1->Init(FIntVector2(Space->GetTileMapPos().X, Space->GetTileMapPos().Y), Space->Width, SplitPoint, GetMinRoomWidth(), GetMinRoomHeight());
        TObjectPtr<UCCRoom> Room2 = NewObject<UCCRoom>();
        Room2->Init(FIntVector2(Space->GetTileMapPos().X, Space->GetTileMapPos().Y + SplitPoint), Space->Width, Space->Height - SplitPoint, GetMinRoomWidth(), GetMinRoomHeight());
        SplitSpace(OutRooms, Room1, MinWidth, MinHeight, Depth - 1);
        SplitSpace(OutRooms, Room2, MinWidth, MinHeight, Depth - 1);
    }
}

void ACCTileMapActor::GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight) const
{
    TObjectPtr<UCCRoom> InitialSpace = NewObject<UCCRoom>();
    InitialSpace->Init(FIntVector2(0, 0), MapWidth, MapHeight, MinRoomWidth, MinRoomHeight);
    int32 MaxDepth = 20; // 재귀 깊이 조정
    SplitSpace(OutRooms, InitialSpace, MinWidth, MinHeight, MaxDepth);
}

float ACCTileMapActor::CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2) const
{

    FVector2D StartPos0 = FVector2D(Platform0.GetStartPos().X, Platform0.GetStartPos().Y);
    FVector2D EndPos0 = FVector2D(Platform0.GetEndPos().X, Platform0.GetEndPos().Y);
    FVector2D StartPos1 = FVector2D(Platform1.GetStartPos().X, Platform1.GetStartPos().Y);
    FVector2D EndPos1 = FVector2D(Platform1.GetEndPos().X, Platform1.GetEndPos().Y);

    if (StartPos0.X >= EndPos1.X)
    {
        Pos1 = Platform0.GetStartPos();
        Pos2 = Platform1.GetStartPos();
        return  FVector2D::Distance(StartPos0, StartPos1);
    }
    else if (EndPos0.X <= StartPos1.X)
    {
        Pos1 = Platform0.GetEndPos();
        Pos2 = Platform1.GetStartPos();
        return FVector2D::Distance(EndPos0, StartPos1);
    }
    else if (StartPos0.X <= EndPos1.X && StartPos0.X >= StartPos1.X)
    {
        Pos1 = Platform0.GetStartPos();
        Pos2 = FIntVector2(Platform0.GetStartPos().X, Platform1.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }
    else if (EndPos0.X <= EndPos1.X && EndPos0.X >= StartPos1.X)
    {
        Pos1 = Platform0.GetEndPos();
        Pos2 = FIntVector2(Platform0.GetEndPos().X, Platform1.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }
    else if (StartPos0.X <= StartPos1.X && EndPos0.X >= EndPos1.X)
    {
        Pos1 = FIntVector2((Platform1.GetStartPos().X + Platform1.GetEndPos().X) / 2, Platform1.GetStartPos().Y);
        Pos2 = FIntVector2((Platform1.GetStartPos().X + Platform1.GetEndPos().X) / 2, Platform0.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }
    else if (StartPos0.X >= StartPos1.X && EndPos0.X <= EndPos1.X)
    {
        Pos1 = FIntVector2((Platform0.GetStartPos().X + Platform0.GetEndPos().X) / 2, Platform0.GetStartPos().Y);
        Pos2 = FIntVector2((Platform0.GetStartPos().X + Platform0.GetEndPos().X) / 2, Platform1.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }

    return -1.0f;
}


void ACCTileMapActor::GenerateMST(TArray<UCCPlatform*>& PlatformArr) const
{
    if (PlatformArr.Num() < 2)
    {
        return; // 플랫폼이 두 개 미만일 경우 MST를 생성할 수 없음
    }

    TArray<FPlatformEdge> Edges;

    // 모든 플랫폼 쌍의 거리를 계산하여 간선 목록을 생성
    for (int32 i = 0; i < PlatformArr.Num(); ++i)
    {
        for (int32 j = i + 1; j < PlatformArr.Num(); ++j)
        {
            FIntVector2 Pos1;
            FIntVector2 Pos2;
            float Distance = CalculatePlatformDistance(*PlatformArr[i], *PlatformArr[j], Pos1, Pos2);
            Edges.Add(FPlatformEdge(i, j, Distance, Pos1, Pos2));
        }
    }

    // 간선을 가중치(거리)에 따라 정렬
    Edges.Sort();

    // Union-Find 구조체 초기화
    TObjectPtr<UCCUnionFind> UnionFind;
    UnionFind = NewObject<UCCUnionFind>();
    UnionFind->Init(PlatformArr.Num());

    TArray<FPlatformEdge> MST;
    MST.Reserve(PlatformArr.Num() - 1);

    // 크루스칼 알고리즘을 이용하여 MST 생성
    for (const FPlatformEdge& E : Edges)
    {
        if (UnionFind->Find(E.PlatformIndex1) != UnionFind->Find(E.PlatformIndex2))
        {
            MST.Add(E);
            UnionFind->Union(E.PlatformIndex1, E.PlatformIndex2);

            // MST의 간선 수가 (N-1)개가 되면 완료
            if (MST.Num() == PlatformArr.Num() - 1)
            {
                break;
            }
        }
    }

    // MST 결과를 사용하여 플랫폼을 연결
    for (const FPlatformEdge& E : MST)
    {
        if (E.Weight >= 8.0f)
        {
            CreatePlatformsAlongEdge(PlatformArr, E);
        }
    }
}

void ACCTileMapActor::CreatePlatformsAlongEdge(TArray<UCCPlatform*>& PlatformArr, const FPlatformEdge& Edge) const
{
    FVector2D FloatPos1(Edge.Pos1.X, Edge.Pos1.Y);
    FVector2D FloatPos2(Edge.Pos2.X, Edge.Pos2.Y);

    FVector2D Direction = (FloatPos2 - FloatPos1).GetSafeNormal();
    float Distance = Edge.Weight;
    int32 NumOfPlatform = Distance / 8;

    for (float CurrentDistance = Distance / (NumOfPlatform + 1); CurrentDistance < Distance; CurrentDistance += Distance / (NumOfPlatform + 1))
    {
        FVector2D NewPlatformPos = FloatPos1 + Direction * CurrentDistance;
        int32 NewLength = FMath::RandRange(4, 6);
        NewPlatformPos.X -= NewLength / 2;
        if (NewPlatformPos.X < 0)
        {
            NewPlatformPos.X = 0;
        }
        // 새 플랫폼 생성
        TObjectPtr<UCCPlatform> NewPlatform = NewObject<UCCPlatform>();
        NewPlatform->Init(FIntVector2(NewPlatformPos.X, NewPlatformPos.Y), NewLength);
        PlatformArr.Add(NewPlatform);
    }
}

void ACCTileMapActor::InitializeTileMap(UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight)
{
    TileMapHeight = Rows;
    TileMapWidth = Columns;
    TileWidth = NewTileWidth;
    TileHeight = NewTileHeight;

    if (FieldTileMapComponent && TileSet)
    {
        FieldTileMapComponent->CreateNewTileMap(Columns, Rows, TileWidth, TileHeight, 1.0f, true);
        FieldTileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
        FieldTileMapComponent->SetRelativeLocation(FVector(0, UCCLayerManager::GetFieldTileY(), 0));

        for (int32 Row = 0; Row < Rows; ++Row)
        {
            for (int32 Column = 0; Column < Columns; ++Column)
            {
                FPaperTileInfo TileInfo;
                TileInfo.TileSet = TileSet;
                TileInfo.PackedTileIndex = (int32)ETileType::None;
                SetTileIfPossible(Column, Row, 0, TileInfo, false);
            }
        }
    }
}

bool ACCTileMapActor::SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly)
{
    if (FieldTileMapComponent)
    {
        if (Column >= 0 && Column < FieldTileMapComponent->TileMap->MapWidth && Row >= 0 && Row < FieldTileMapComponent->TileMap->MapHeight)
        {
            if (!bEmptyOnly || FieldTileMapComponent->GetTile(Column, Row, Layer).GetTileIndex() == (int32)ETileType::None)
            {
                FieldTileMapComponent->SetTile(Column, Row, Layer, TileInfo);
                return true;
            }
        }
    }
    return false;
}

void ACCTileMapActor::CreatFieldTile(UCCField* Field)
{
    if (Field->GetFieldType() != EFieldType::BasicField)
    {
        SetTileIfPossible(Field->GetTileMapPos().X, Field->GetTileMapPos().Y, 0, UCCStageMapManager::GetInstance()->TileSetPerFieldDic.Find(Field->GetFieldType())->LeftTile, false);
        for (int32 i = Field->GetTileMapPos().X + 1; i < Field->GetTileMapPos().X + Field->GetLength() - 1; i++)
        {
            SetTileIfPossible(i, Field->GetTileMapPos().Y, 0, UCCStageMapManager::GetInstance()->TileSetPerFieldDic.Find(Field->GetFieldType())->MiddleTile);
        }
        SetTileIfPossible(Field->GetTileMapPos().X + Field->GetLength() - 1, Field->GetTileMapPos().Y, 0, UCCStageMapManager::GetInstance()->TileSetPerFieldDic.Find(Field->GetFieldType())->RightTile, false);
    }
    else
    {
        for (int32 i = Field->GetTileMapPos().X; i < Field->GetTileMapPos().X + Field->GetLength(); i++)
        {
            SetTileIfPossible(i, Field->GetTileMapPos().Y, 0, *UCCStageMapManager::GetInstance()->TileInfoPerTileDic.Find(ETileType::Basic));
        }
    }

    if (HasAuthority())
    {
        SetupTileColliders(Field->GetTileMapPos().X, Field->GetTileMapPos().Y, Field->GetLength(), Field->GetFieldType());
    }
}

void ACCTileMapActor::SetupTileColliders(int32 Column, int32 Row, int32 ColliderLength, EFieldType NewFieldType)
{
    int32 TileSize = GetTileHeight();

    UCCFieldCollider* FieldCollider = NewObject<UCCFieldCollider>(this);
    FieldCollider->RegisterComponent();
    FVector RelativeLocation(Column * TileSize + TileSize / 2 * ColliderLength - TileSize / 2.0f, 0, -Row * TileSize);
    FVector Extent(TileSize / 2 * ColliderLength, 1000.0f, TileSize / 2);
    FieldCollider->Init(NewFieldType, RelativeLocation, Extent);
    FieldCollider->SetIsReplicated(true);
    FieldCollider->AttachToComponent(FieldTileMapComponent, FAttachmentTransformRules::KeepRelativeTransform);
    this->AddInstanceComponent(FieldCollider);
    FieldColliderArr.Add(FieldCollider);

    UCCFieldTrigger* FieldTrigger = NewObject<UCCFieldTrigger>(this);
    FieldTrigger->RegisterComponent();
    RelativeLocation.Z -= TileSize * 0.5f;
    FieldTrigger->Init(RelativeLocation, Extent);
    FieldTrigger->SetIsReplicated(true);
    FieldTrigger->AttachToComponent(FieldTileMapComponent, FAttachmentTransformRules::KeepRelativeTransform);
    this->AddInstanceComponent(FieldTrigger);

    FieldTriggerArr.Add(FieldTrigger);
}

void ACCTileMapActor::OnRep_FieldColliderArr()
{
    for (UCCFieldCollider* FieldCollider : FieldColliderArr)
    {
        if (FieldCollider != nullptr)
        {
            UCCStageMapManager::GetInstance()->TurnTrueIsColliderGenerated();
            FieldCollider->AttachToComponent(FieldTileMapComponent, FAttachmentTransformRules::KeepRelativeTransform);
            this->AddInstanceComponent(FieldCollider);
        }
    }
}
    

void ACCTileMapActor::OnRep_FieldTriggerArr()
{
    for (UCCFieldTrigger* FieldTrigger : FieldTriggerArr)
    {
        if (FieldTrigger != nullptr)
        {
            UCCStageMapManager::GetInstance()->TurnTrueIsTriggerGenerated();
            FieldTrigger->AttachToComponent(FieldTileMapComponent, FAttachmentTransformRules::KeepRelativeTransform);
            this->AddInstanceComponent(FieldTrigger);
        }
    }
}

void ACCTileMapActor::PlaceSpriteEachField(UCCField* Field)
{
    switch (Field->GetFieldType())
    {
    case EFieldType::BasicField:
        PlaceSprites(Field, 0.5f, EFeatureType::GrassFeature, false, true);
        PlaceSprites(Field, 0.5f, EFeatureType::PlantFeature, false, true);
        break;
    case EFieldType::WatersideField:
        PlaceSprites(Field, 0.5, EFeatureType::WeedFeature, true, true);
        PlaceSprites(Field, 0.5, EFeatureType::StoneFeature, true, true);
        break;
    case EFieldType::AsphaltField:
        PlaceSprites(Field, 4.0, EFeatureType::BuildingFeature, true, false);
        PlaceSprites(Field, 0.5, EFeatureType::CarFeature, false, true);
        PlaceSprites(Field, 0.5, EFeatureType::LightFeature, false, true);
        break;
    case EFieldType::CaveField:
        PlaceSprites(Field, 1.0, EFeatureType::RatCaveFeature, true, false, true, 4, 4);
        PlaceSprites(Field, 0.5, EFeatureType::RockFeature, false, true);
        break;
    case EFieldType::HillField:
        
        break;
    case EFieldType::RaccoonHouseField:
        PlaceSprites(Field, 1.0, EFeatureType::RacconHouseFeature, true, true, false, 1, 1);
        PlaceSprites(Field, 0.5, EFeatureType::RockFeature, false, true);
        break;
    case EFieldType::DogHouseField:
        PlaceSprites(Field, 1.0, EFeatureType::DogHouseFeature, true, true, false, 1, 1);
        PlaceSprites(Field, 0.5, EFeatureType::RockFeature, false, true);
        break;
    default:
        break;
    }
}

void ACCTileMapActor::PlaceSprites(UCCField* Field, float TileInterval, EFeatureType FeatureType, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum)
{
    const FCCFeatureInfoArrContainer& FeatureInfoArr = *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(FeatureType);
    int32 OffsetTiles = Field->GetLength() - 1;

    int32 SpriteNum = 0;

    int32 PossibleOffset = OffsetTiles / TileInterval;
    TArray<int32> NecessarySpriteOffset;
    while (MinSpriteNum > NecessarySpriteOffset.Num())
    {
        int32 Offset = FMath::RandRange(0, PossibleOffset - 1);
        if (NecessarySpriteOffset.Contains(Offset) == false)
        {
            NecessarySpriteOffset.Add(Offset);
        }
    }

    for (int32 Offset : NecessarySpriteOffset)
    {
        int32 Index = FeatureInfoArr.GetRandomIndex();
        if (Index != 0)
        {
            float RandomScale = FMath::RandRange(1.0f, 1.5f);
            FVector LocalPos = CalculateSpriteWorldSpacePos(GetWorldSpaceStartPos(Field).X + GetTileWidth() * Offset, GetWorldSpaceStartPos(Field), FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, bIsBeforePlayer);
            FBox2D BoxForSprite = GetSpriteBox(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, RandomScale, LocalPos);
            if ((bAllowOverlap || !RootNode->IsColliding(BoxForSprite)))
            {
                if (SpriteNum >= MaxSpriteNum) return;
                SpriteNum++;
                CreateAndAttachSpriteComponent(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale, bIsBeforePlayer);
                SpriteComponentInfoArr.Add(FSpriteComponentInfo(FeatureType, Index, LocalPos, RandomScale, bIsBeforePlayer));
            }
        }
    }

    do
    {
        for (float i = GetWorldSpaceStartPos(Field).X; i <= GetWorldSpaceEndPos(Field).X; i += GetTileWidth() * TileInterval)
        {
            int32 Index = FeatureInfoArr.GetRandomIndex();
            if (Index != 0)
            {
                float RandomScale = FMath::RandRange(1.0f, 1.0f);
                FVector LocalPos = CalculateSpriteWorldSpacePos(i, GetWorldSpaceStartPos(Field), FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, bIsBeforePlayer);
                FBox2D BoxForSprite = GetSpriteBox(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, RandomScale, LocalPos);
                if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
                {
                    if (SpriteNum >= MaxSpriteNum) return;
                    SpriteNum++;
                    CreateAndAttachSpriteComponent(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale, bIsBeforePlayer);
                    SpriteComponentInfoArr.Add(FSpriteComponentInfo(FeatureType, Index, LocalPos, RandomScale, bIsBeforePlayer));
                }
            }
        }
    } while (SpriteNum < MinSpriteNum);
}

FBox2D ACCTileMapActor::GetSpriteBox(UPaperSprite* PaperSprite, float Scale, FVector MiddleButtonPos)
{
    if (PaperSprite)
    {
        FVector2D LeftDown(FVector2D(MiddleButtonPos.X - PaperSprite->GetBakedTexture()->GetSizeX() / 2, MiddleButtonPos.Z));
        FVector2D RightUp(FVector2D(MiddleButtonPos.X + PaperSprite->GetBakedTexture()->GetSizeX() / 2, MiddleButtonPos.Z + PaperSprite->GetBakedTexture()->GetSizeY() / 2));
        FBox2D BoxForSprite(LeftDown, RightUp);
        return BoxForSprite;
    }
    return FBox2D();
}

void ACCTileMapActor::CreateAndAttachSpriteComponent(UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale, bool bIsBeforePlayer)
{
    if (!FeatureSprite)
    {
        return;
    }

    UPaperSpriteComponent* NewSpriteComponent = NewObject<UPaperSpriteComponent>(FieldTileMapComponent->GetOwner());
    if (NewSpriteComponent)
    {
        NewSpriteComponent->RegisterComponent();
        NewSpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        this->AddInstanceComponent(NewSpriteComponent);
        NewSpriteComponent->SetSprite(FeatureSprite);
        NewSpriteComponent->SetRelativeLocation(LocalPos);
        NewSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));
        NewSpriteComponent->SetRelativeScale3D(FVector(RandomScale, 1, RandomScale));
        NewSpriteComponent->SetIsReplicated(true);
        if (bIsBeforePlayer)
        {
            NewSpriteComponent->SetTranslucentSortPriority(GetBeforePlayerOrder());
            AddBeforPlayerOrder();
        }
        else
        {
            NewSpriteComponent->SetTranslucentSortPriority(GetAfterPlayerOrder());
            AddAfterPlayerOrder();
        }

        if (bAddToCollisionTree)
        {
            RootNode->Insert(BoxForSprite);
        }
    }
}

FVector ACCTileMapActor::CalculateSpriteWorldSpacePos(float XPos, FVector StartPos, UPaperSprite* FeatureSprite, bool bIsBeforePlayer)
{
    float YPos;
    if (bIsBeforePlayer)
    {
        YPos = UCCLayerManager::GetBeforePlayerSpriteY();
    }
    else
    {
        YPos = UCCLayerManager::GetAfterPlayerSpriteY();
    }

    return FVector(XPos, YPos - GetActorLocation().Y, GetTileHeight() / 2 + StartPos.Z);
}

void ACCTileMapActor::OnRep_SpriteComponentInfoArr()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), SpriteComponentInfoArr.Num()));

    for (int32 i = 0; i < SpriteComponentInfoArr.Num(); i++)
    {
        UPaperSpriteComponent* NewSpriteComponent = NewObject<UPaperSpriteComponent>(this);
        FCCFeatureInfoArrContainer* InfoArr = UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(SpriteComponentInfoArr[i].FeatureType);
        const FCCFeatureInfo& Info = InfoArr->FeatureInfoArr[SpriteComponentInfoArr[i].SpriteIndex];
        if (!Info.FeatureSprite)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FeatureSprite is nullptr!"));
        }
        NewSpriteComponent->SetSprite(Info.FeatureSprite);
        NewSpriteComponent->SetRelativeLocation(SpriteComponentInfoArr[i].RelativePos);
        NewSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));
        NewSpriteComponent->SetRelativeScale3D(FVector(SpriteComponentInfoArr[i].RandomScale, 1, SpriteComponentInfoArr[i].RandomScale));
        if (SpriteComponentInfoArr[i].bIsBeforePlayer)
        {
            NewSpriteComponent->SetTranslucentSortPriority(GetBeforePlayerOrder());
            AddBeforPlayerOrder();
        }
        else
        {
            NewSpriteComponent->SetTranslucentSortPriority(GetAfterPlayerOrder());
            AddAfterPlayerOrder();
        }
        NewSpriteComponent->RegisterComponent();
        NewSpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        this->AddInstanceComponent(NewSpriteComponent);
    }

    UCCStageMapManager::GetInstance()->TurnTrueIsSpriteGenerated();
}

FSpawnableField ACCTileMapActor::ChangeIntoSpawnableField(UCCField* Field, ESpawnableType SpawnableType, int32 MaxCharacterNum)
{
    FVector StartPos = GetWorldSpaceStartPos(Field);
    StartPos.Z += GetTileWidth();
    FVector EndPos = GetWorldSpaceEndPos(Field);
    EndPos.Z += GetTileWidth();
    return FSpawnableField(StartPos, EndPos, SpawnableType, MaxCharacterNum);
}

void ACCTileMapActor::OnRep_Trees()
{
}

void ACCTileMapActor::OnRep_RedPlayerStartPos()
{
    RedPlayerSpawner->SetActorLocation(RedPlayerStartPos);
}

void ACCTileMapActor::OnRep_BluePlayerStartPos()
{
    BluePlayerSpawner->SetActorLocation(BluePlayerStartPos);
}

void ACCTileMapActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherComp || !OtherComp->IsValidLowLevel())
    {
        return;
    }

    if (OtherComp->IsA(UDamageSphereComponent::StaticClass()))
    {
        return;
    }

    TScriptInterface<ICCRespawnable> Respawnable = TScriptInterface<ICCRespawnable>(OtherActor);

    if (Respawnable)
    {
        Respawnable->OnDeath();
    }
    else
    {
        TScriptInterface<ICCDestroyable> Destroyable = TScriptInterface<ICCDestroyable>(OtherActor);
        if (Destroyable)
        {
            Destroyable->OnDeath(this);
        }
        else
        {
            OtherActor->Destroy();
        }
    }
}