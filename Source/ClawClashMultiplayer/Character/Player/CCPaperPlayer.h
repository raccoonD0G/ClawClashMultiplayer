// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/Character/CCPaperCharacter.h"
#include "InputActionValue.h"
#include "CCPaperPlayer.generated.h"



UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Jump UMETA(DisplayName = "Jump"),
	Move UMETA(DisplayName = "Move"),
	Falling UMETA(DisplayName = "Falling"),
	Land UMETA(DisplayName = "Land")
};

class UHealthComponent;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPaperPlayer : public ACCPaperCharacter
{
	GENERATED_BODY()
	
public:
	ACCPaperPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCurrentState(EPlayerState NewState);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void UpdateIdle();
	void UpdateMove();
	void UpdateJump();
	void UpdateLand();
	void UpdateFalling();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float PlayerIdleThreshold = 0.1f;
	bool ShouldJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlayerState CurrentState;

// Transform Section
protected:
	UPROPERTY(ReplicatedUsing = OnRep_ServerPlayerYaw)
	float ServerPlayerYaw;
	
	UFUNCTION()
	void OnRep_ServerPlayerYaw();

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
//Input Section
protected:

	float JumpStrength = 0.0f;
	float JumpHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	
	void LeftRightMove(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void Server_LeftRightMove(const FVector2D InputVector);
	void Server_LeftRightMove_Implementation(const FVector2D InputVector);

	void ReadyJump();
	virtual void Jump() override;

//Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> LandAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> FallingAnimation;

// Damage Section
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause) override;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

};
