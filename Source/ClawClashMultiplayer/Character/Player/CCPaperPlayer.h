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
	ReadyJump UMETA(DisplayName = "ReadyJump"),
	KeepReadyJump UMETA(DisplayName = "KeepReadyJump"),
	Move UMETA(DisplayName = "Move"),
	Falling UMETA(DisplayName = "Falling"),
	KeepFalling UMETA(DisplayName = "KeepFalling"),
	Land UMETA(DisplayName = "Land"),
	Attack
};

class UHealthComponent;
class UWeaponComponent;

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

	void SetAnimation();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void UpdateIdle();
	void UpdateMove();
	void UpdateReadyJump(float DeltaTime);
	void UpdateKeepReadyJump(float DeltaTime);
	void UpdateJump();
	void UpdateLand();
	void UpdateFalling();
	void UpdateKeepFalling();
	void UpdateAttack();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float PlayerIdleThreshold = 0.1f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState, EditAnywhere, BlueprintReadOnly)
	EPlayerState CurrentState;

	UFUNCTION()
	void OnRep_CurrentState();

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
	
// Input Section
protected:

	float MaxJumpForce = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	
	void LeftRightMove(const FInputActionValue& Value);
	void LeftRightMoveCompleted(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_LeftRightMove(const FVector2D InputVector);
	void Server_LeftRightMove_Implementation(const FVector2D InputVector);

	UFUNCTION(Server, Reliable)
	void Server_LeftRightMoveCompleted(const FVector2D InputVector);
	void Server_LeftRightMoveCompleted_Implementation(const FVector2D InputVector);

	UFUNCTION(Server, Reliable)
	void Server_StartReadyJump();
	void Server_StartReadyJump_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_ReleaseJump();
	void Server_ReleaseJump_Implementation();

	float JumpReadyTime = 0.0f;
	const float MaxReadyTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PreviousInputVector;

//Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> ReadyJumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> KeepReadyJumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> LandAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> FallingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> KeepFallingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> AttackAnimation;

// Damage Section
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause) override;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

// Upgrade Section
protected:
	TArray<TFunctionRef<float()>> JumpPowerMultipliers;

	TArray<TFunctionRef<float()>> MoveSpeedMultipliers;

	TArray<TFunctionRef<float()>> AttackPowerMultipliers;

	TArray<TFunctionRef<float()>> OccupySpeedMultipliers;

public:
	FORCEINLINE void AddJumpPowerMultipliers(TFunctionRef<float()> Func) { JumpPowerMultipliers.Add(Func); }
	FORCEINLINE void AddMoveSpeedMultipliers(TFunctionRef<float()> Func) { MoveSpeedMultipliers.Add(Func); }
	FORCEINLINE void AddAttackPowerMultipliers(TFunctionRef<float()> Func) { AttackPowerMultipliers.Add(Func); }
	FORCEINLINE void AddOccupySpeedMultipliers(TFunctionRef<float()> Func) { OccupySpeedMultipliers.Add(Func); }

// Attack Section
protected:
	UPROPERTY()
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_Attack(const FInputActionValue& Value);
	void Server_Attack_Implementation(const FInputActionValue& Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Attack(const FInputActionValue& Value);
	void Multicast_Attack_Implementation(const FInputActionValue& Value);
};
