// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"

class UPaperFlipbookComponent;
class UCapsuleComponent;
class ICCAttacker;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASHMULTIPLAYER_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Attack Section
public:
	TScriptInterface<ICCAttacker> Attacker;

	void DoAttack();

	UFUNCTION()
	void EndAttack();

// Component Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USceneComponent> WeaponCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UPaperFlipbookComponent> WeaponFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UCapsuleComponent> WeaponCollider;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponCollider(UCapsuleComponent* InWeaponCollider);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE void SetWeaponCenter(USceneComponent* InWeaponCenter) { WeaponCenter = InWeaponCenter; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE void SetWeaponFlipbook(UPaperFlipbookComponent* InWeaponFlipbook) { WeaponFlipbook = InWeaponFlipbook; }

public:
	float GetWeaponFlipbookLengthInSeconds() const;

// Timeline Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> WeaponRotateFloatCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UTimelineComponent> WeaponCenterTimeline;

	FOnTimelineFloat UpdateTimelineFloatTrack;
	FOnTimelineEvent OnTimelineEnd;

	UFUNCTION()
	void UpdateWeaponCenterRotation(float Output);

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
