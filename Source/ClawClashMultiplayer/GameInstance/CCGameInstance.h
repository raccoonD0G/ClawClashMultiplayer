// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "CCGameInstance.generated.h"


class UCCStageMapManager;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UCCGameInstance();

    virtual void Init() override;
    virtual void OnStart() override;
    virtual void Shutdown() override;
};
