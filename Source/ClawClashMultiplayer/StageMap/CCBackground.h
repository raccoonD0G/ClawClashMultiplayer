// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCBackground.generated.h"

UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCBackground : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCBackground();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Background Section
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<class UPaperSpriteComponent> BackgroundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    TObjectPtr<class ACCTileMapActor> StageMap;

    UPROPERTY()
    TObjectPtr<class UPaperSprite> BackGroundSprite;

    UPROPERTY(ReplicatedUsing = OnRep_BackGroundSpriteIndex)
    int32 BackGroundSpriteIndex;

    UFUNCTION()
    void OnRep_BackGroundSpriteIndex();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TArray<TObjectPtr<class UPaperSprite>> BackGroundSpriteArr;

    UPROPERTY()
    TObjectPtr<class ACharacter> Player;

    float UpSizeAmount;
};
