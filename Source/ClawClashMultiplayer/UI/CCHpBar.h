// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCWorldSpaceWidget.h"
#include "CCHpBar.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCHpBar : public UCCWorldSpaceWidget
{
	GENERATED_BODY()
	
public:
	void Init(UHealthComponent* InHealthComponent);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateHpBar(int32 NewCurrentHealth, int32 MaxHp);

	UPROPERTY()
	TWeakObjectPtr<UHealthComponent> HealthComponent; 
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HpBar;
};
