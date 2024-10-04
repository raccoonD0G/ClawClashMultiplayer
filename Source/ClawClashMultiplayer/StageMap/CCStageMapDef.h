// CCTile.h
#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class ETileType : uint8
{
    Empty UMETA(DisplayName = "Empty"),
    Basic UMETA(DisplayName = "Basic"),
    WatersideLeft UMETA(DisplayName = "Waterside"),
    Waterside UMETA(DisplayName = "Waterside"),
    WatersideRight UMETA(DisplayName = "Waterside"),
    AsphaltLeft UMETA(DisplayName = "Asphalt"),
    Asphalt UMETA(DisplayName = "Asphalt"),
    AsphaltRight UMETA(DisplayName = "Asphalt"),
    CaveLeft UMETA(DisplayName = "Cave"),
    Cave UMETA(DisplayName = "Cave"),
    CaveRight UMETA(DisplayName = "Cave"),
    HillLeft UMETA(DisplayName = "Hill"),
    Hill UMETA(DisplayName = "Hill"),
    HillRight UMETA(DisplayName = "Hill"),
    HillSpace UMETA(DisplayName = "HillSpace"),
    None UMETA(DisplayName = "None"),
    
};

UENUM(BlueprintType)
enum class EFieldType : uint8
{
    BasicField  UMETA(DisplayName = "BasicField"),
    WatersideField UMETA(DisplayName = "Waterside"),
    AsphaltField UMETA(DisplayName = "Asphalt"),
    CaveField UMETA(DisplayName = "Cave"),
    HillField UMETA(DisplayName = "Hill"),
    RaccoonHouseField UMETA(DisplayName = "RaccoonHouse"),
    DogHouseField UMETA(DisplayName = "DogHouse")
};

// Feature
UENUM(BlueprintType)
enum class EBuildingFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    House0Feature UMETA(DisplayName = "House0Feature"),
    House1Feature UMETA(DisplayName = "House1Feature"),
    House2Feature UMETA(DisplayName = "House2Feature"),
    House3Feature UMETA(DisplayName = "House3Feature"),
    House4Feature UMETA(DisplayName = "House4Feature"),
    House5Feature UMETA(DisplayName = "House5Feature"),
    House6Feature UMETA(DisplayName = "House6Feature"),
};

UENUM(BlueprintType)
enum class ECarFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Car0Feature UMETA(DisplayName = "Car0"),
    Car1Feature UMETA(DisplayName = "Car1"),
};

UENUM(BlueprintType)
enum class EFeatureType : uint8
{
    BuildingFeature UMETA(DisplayName = "BuildingFeature"),
    CarFeature UMETA(DisplayName = "CarFeature"),
    WeedFeature UMETA(DisplayName = "WeedFeature"),
    LightFeature UMETA(DisplayName = "LightFeature"),
    RockFeature UMETA(DisplayName = "RockFeature"),
    StoneFeature UMETA(DisplayName = "StoneFeature"),
    PlantFeature UMETA(DisplayName = "PlantFeature"),
    GrassFeature UMETA(DisplayName = "GrassFeature"),
    WaterFeature UMETA(DisplayName = "WaterFeature"),
    RatCaveFeature UMETA(DisplayName = "RatCaveFeature"),
    RacconHouseFeature UMETA(DisplayName = "RacconHouseFeature"),
    DogHouseFeature UMETA(DisplayName = "RacconHouseFeature"),
    TreeFeature UMETA(DisplayName = "TreeFeature"),
};

UENUM(BlueprintType)
enum class EWeedFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Weed0 UMETA(DisplayName = "Weed0"),
    Weed1 UMETA(DisplayName = "Weed1"),
    Weed2 UMETA(DisplayName = "Weed2"),
    Weed3 UMETA(DisplayName = "Weed3")
};

UENUM(BlueprintType)
enum class ELightFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Light0 UMETA(DisplayName = "Light0"),
    Light1 UMETA(DisplayName = "Light1"),
    Light2 UMETA(DisplayName = "Light2"),
};

UENUM(BlueprintType)
enum class ERockFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Rock0 UMETA(DisplayName = "Rock0"),
    Rock1 UMETA(DisplayName = "Rock1"),
    Rock2 UMETA(DisplayName = "Rock2"),
    Rock3 UMETA(DisplayName = "Rock3"),
    Rock4 UMETA(DisplayName = "Rock4"),
};

UENUM(BlueprintType)
enum class EStoneFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Stone0 UMETA(DisplayName = "Stone0"),
    Stone1 UMETA(DisplayName = "Stone1"),
    Stone2 UMETA(DisplayName = "Stone2"),
    Stone3 UMETA(DisplayName = "Stone3"),
};

UENUM(BlueprintType)
enum class EPlantFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Plant0 UMETA(DisplayName = "Plant0"),
    Plant1 UMETA(DisplayName = "Plant1"),
    Plant2 UMETA(DisplayName = "Plant2"),
    Plant3 UMETA(DisplayName = "Plant3"),
    Plant4 UMETA(DisplayName = "Plant4"),
    Plant5 UMETA(DisplayName = "Plant5"),
    Plant6 UMETA(DisplayName = "Plant6"),
};

UENUM(BlueprintType)
enum class EGrassFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Grass0 UMETA(DisplayName = "Grass0"),
    Grass1 UMETA(DisplayName = "Grass1"),
    Grass2 UMETA(DisplayName = "Grass2"),
    Grass3 UMETA(DisplayName = "Grass3"),
};

UENUM(BlueprintType)
enum class EWaterFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Water0Feature UMETA(DisplayName = "Water0Feature"),

};

UENUM(BlueprintType)
enum class ERatCaveFeature : uint8
{
    None UMETA(DisplayName = "None"),
    RatCave0 UMETA(DisplayName = "RatCave0"),
    RatCave1 UMETA(DisplayName = "RatCave1"),
    RatCave2 UMETA(DisplayName = "RatCave2"),
    RatCave3 UMETA(DisplayName = "RatCave3"),
    RatCave4 UMETA(DisplayName = "RatCave4"),
    RatCave5 UMETA(DisplayName = "RatCave5"),
};

UENUM(BlueprintType)
enum class ERaccooHouseFeature : uint8
{
    None UMETA(DisplayName = "None"),
    RaccoonHouse0 UMETA(DisplayName = "RaccoonHouse0"),
    RaccoonHouse1 UMETA(DisplayName = "RaccoonHouse1"),
    RaccoonHouse2 UMETA(DisplayName = "RaccoonHouse2"),
    RaccoonHouse3 UMETA(DisplayName = "RaccoonHouse3"),
    RaccoonHouse4 UMETA(DisplayName = "RaccoonHouse4"),
    RaccoonHouse5 UMETA(DisplayName = "RaccoonHouse5"),
};

UENUM(BlueprintType)
enum class EDogHouseFeature : uint8
{
    None UMETA(DisplayName = "None"),
    DogHouse0 UMETA(DisplayName = "EDogHouse0"),
    DogHouse1 UMETA(DisplayName = "EDogHouse1"),
    DogHouse2 UMETA(DisplayName = "EDogHouse2"),
    DogHouse3 UMETA(DisplayName = "EDogHouse3"),
};

UENUM(BlueprintType)
enum class ETreeFeature : uint8
{
    None UMETA(DisplayName = "None"),
    Tree0 UMETA(DisplayName = "Tree0"),
    Tree1 UMETA(DisplayName = "Tree1")
};