// Fill out your copyright notice in the Description page of Project Settings.


#include "CCBackground.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Character.h"
#include "PaperSprite.h"
#include "ClawClashMultiplayer/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClashMultiplayer/StageMap/CCTileMapActor.h"
#include "ClawClashMultiplayer/Managers/LayerManager/CCLayerManager.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACCBackground::ACCBackground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    UpSizeAmount = 0.5f;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    RootComponent = BackgroundComponent;
    bReplicates = true;
    BackGroundSpriteIndex = 0;
}

// Called when the game starts or when spawned
void ACCBackground::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        Player = Cast<ACCPaperPlayer>(PlayerController->GetPawn());
    }

    if (HasAuthority())
    {
        int32 Index = FMath::RandRange(0, BackGroundSpriteArr.Num() - 1);
        BackGroundSpriteIndex = Index;
    }
    else
    {
        if (BackgroundComponent)
        {
            BackGroundSprite = BackGroundSpriteArr[BackGroundSpriteIndex];

            float TileMapWidthInPixels = StageMap->GetTileWidth() * StageMap->GetTileMapWidth();
            float TileMapHeightInPixels = StageMap->GetTileHeight() * StageMap->GetTileMapHeight();

            BackgroundComponent->SetSprite(BackGroundSprite);

            float SpriteWidth = BackGroundSprite->GetBakedTexture()->GetSizeX();
            float SpriteHeight = BackGroundSprite->GetBakedTexture()->GetSizeY();

            BackgroundComponent->SetWorldScale3D(FVector(TileMapWidthInPixels / SpriteWidth * UpSizeAmount, 1.0f, TileMapHeightInPixels / SpriteHeight * UpSizeAmount));
            BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
        }
    }
}

void ACCBackground::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACCBackground, BackGroundSpriteIndex);
}

// Called every frame
void ACCBackground::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!Player)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            Player = Cast<ACCPaperPlayer>(PlayerController->GetPawn());
        }
        if (!Player)
        {
            return;
        }
    }

    float TileMapWidthInPixels = StageMap->GetTileWidth() * StageMap->GetTileMapWidth();
    float TileMapHeightInPixels = StageMap->GetTileHeight() * StageMap->GetTileMapHeight();

    // Get Player Pos
    FVector PlayerPos = Player->GetActorLocation();
    PlayerPos = PlayerPos - StageMap->GetActorLocation();
    PlayerPos.Z += TileMapHeightInPixels;

    float PlayerStageMapXPos = PlayerPos.X / TileMapWidthInPixels;
    float PlayerStageMapZPos = PlayerPos.Z / TileMapHeightInPixels;

    // Set Background Pos
    float XPos = (0.5f - PlayerStageMapXPos) * (TileMapWidthInPixels - 40960.0f) * UpSizeAmount;
    float ZPos = (0.5f - PlayerStageMapZPos) * (TileMapHeightInPixels - 20480.0f) * UpSizeAmount;

    BackgroundComponent->SetRelativeLocation(FVector(XPos + Player->GetActorLocation().X, UCCLayerManager::GetBackgroundY(), ZPos + Player->GetActorLocation().Z));
}

void ACCBackground::OnRep_BackGroundSpriteIndex()
{
    if (BackgroundComponent)
    {
        BackGroundSprite = BackGroundSpriteArr[BackGroundSpriteIndex];

        float TileMapWidthInPixels = StageMap->GetTileWidth() * StageMap->GetTileMapWidth();
        float TileMapHeightInPixels = StageMap->GetTileHeight() * StageMap->GetTileMapHeight();

        BackgroundComponent->SetSprite(BackGroundSprite);

        float SpriteWidth = BackGroundSprite->GetBakedTexture()->GetSizeX();
        float SpriteHeight = BackGroundSprite->GetBakedTexture()->GetSizeY();

        BackgroundComponent->SetWorldScale3D(FVector(TileMapWidthInPixels / SpriteWidth * UpSizeAmount, 1.0f, TileMapHeightInPixels / SpriteHeight * UpSizeAmount));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
}


