// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCTreeManager.generated.h"

class ACCTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCountsUpdated, int32, RedCount, int32, BlueCount, int32, NeutralCount);

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCTreeManager : public UObject
{
	GENERATED_BODY()
	
public:
	static UCCTreeManager* GetInstance();
	static void DestroyInstance();
protected:
	static UCCTreeManager* Instance;

protected:
	UPROPERTY()
	TSet<TObjectPtr<ACCTree>> RedTrees;

	UPROPERTY()
	TSet<TObjectPtr<ACCTree>> BlueTrees;

	UPROPERTY()
	TSet<TObjectPtr<ACCTree>> NeutralTrees;

public:
	FOnCountsUpdated OnCountUpdateEvent;

	void AddRedTree(ACCTree* NewRedTree);
	void AddBlueTree(ACCTree* NewBlueTree);
	void AddNeutralTree(ACCTree* NewNeutralTree);

	FORCEINLINE int32 GetRedTreeCount() { return RedTrees.Num(); }
	FORCEINLINE int32 GetBlueTreeCount() { return BlueTrees.Num(); }
	FORCEINLINE int32 GetNeutralTreeCount() { return NeutralTrees.Num(); }
	
};
