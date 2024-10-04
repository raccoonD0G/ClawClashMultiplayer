// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "CCUtils.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCUtils : public UObject
{
	GENERATED_BODY()
	
public:
	template <typename T>
	static TArray<T*> GetAllResourceFromFolder(const FString& Path);

    UFUNCTION()
    static int32 GetEnumLength(UEnum* TargetEnum);

    UFUNCTION()
    static TArray<int32> DecomposeNumberToKParts(int32 n, int32 k);

    static int32 GetRandomIndexByProbability(const TArray<float>& Probabilities);
};



template <typename T>
TArray<T*> UCCUtils::GetAllResourceFromFolder(const FString& Path)
{
    TArray<T*> Resources;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    FARFilter Filter;
    Filter.PackagePaths.Add(FName(*Path));
    Filter.ClassPaths.Add(T::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), AssetDataList.Num()));

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager(); // 스트리머블 매니저를 사용하여 에셋 로드

    for (const FAssetData& AssetData : AssetDataList)
    {
        FSoftObjectPath AssetPath = AssetData.ToSoftObjectPath(); // FSoftObjectPath 사용

        // 비동기 로드, 로드된 후에 리소스를 처리할 수 있습니다.
        T* Resource = Cast<T>(Streamable.LoadSynchronous(AssetPath)); // 동기적으로 로드하고 싶다면 LoadSynchronous 사용
        if (Resource)
        {
            Resources.Add(Resource);
        }
    }

    return Resources;
}
