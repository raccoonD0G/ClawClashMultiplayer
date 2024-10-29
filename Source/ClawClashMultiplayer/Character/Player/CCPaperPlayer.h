// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/Character/CCPaperCharacter.h"
#include "InputActionValue.h"
#include "ClawClashMultiplayer/Interfaces/CCAttacker.h"
#include "ClawClashMultiplayer/Interfaces/CCRespawnable.h"
#include "CCPaperPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Jump,
	ReadyJump,
	KeepReadyJump,
	Move,
	Falling,
	KeepFalling,
	Land,
	Attack,
	Died,
};

class UHealthComponent;
class UWeaponComponent;
class UCCPopupWidget;

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCPaperPlayer : public ACCPaperCharacter, public ICCAttacker, public ICCRespawnable
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
	virtual void PossessedBy(AController* NewController) override;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> ReadyJumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> KeepReadyJumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> LandAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> FallingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> KeepFallingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<class UPaperFlipbook> AttackAnimation;

// Damage Section
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause) override;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

// Upgrade Section
protected:
	TArray<TFunction<float()>> JumpPowerMultipliers;

	TArray<TFunction<float()>> MoveSpeedMultipliers;

	TArray<TFunction<float()>> AttackPowerMultipliers;

	TArray<TFunction<float()>> AttackRangeMultipliers;

	TArray<TFunction<float()>> OccupySpeedMultipliers;

public:
	FORCEINLINE void AddJumpPowerMultipliers(TFunction<float()> Func) { JumpPowerMultipliers.Add(Func); }
	FORCEINLINE void AddMoveSpeedMultipliers(TFunction<float()> Func) { MoveSpeedMultipliers.Add(Func); Multicast_ResetMoveSpeed(); }
	FORCEINLINE void AddAttackPowerMultipliers(TFunction<float()> Func) { AttackPowerMultipliers.Add(Func); }
	FORCEINLINE void AddAttackRangeMultipliers(TFunction<float()> Func) { AttackRangeMultipliers.Add(Func); }
	FORCEINLINE void AddOccupySpeedMultipliers(TFunction<float()> Func) { OccupySpeedMultipliers.Add(Func); }

// Attack Section
public:
	FORCEINLINE virtual float GetNormalAttackPower() override { return NormalAttackPower; }
	FORCEINLINE virtual float GetNormalAttackRange() override { return NormalAttackRange; }
	virtual float GetAttackPowerMultiplier() override;
	virtual float GetAttackRangeMultiplier() override;

protected:
	float NormalAttackPower = 20.0f;
	float NormalAttackRange = 1.0f;

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

// Death Section
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCCPopupWidget> DeathWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCCPopupWidget> DeathWidget;

	FOnDeath OnDeathEvent;

	FTimerHandle TeleportTimerHandle;

	UFUNCTION(Server, Reliable)
	void Server_Respawn();
	void Server_Respawn_Implementation();

	void BackToRespawnPos();

	UFUNCTION(Client, Reliable)
	void Client_ShowDeathWidget();
	void Client_ShowDeathWidget_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_CloseDeathWidget();
	void Client_CloseDeathWidget_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetGraySprite();
	void Multicast_SetGraySprite_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetNormalSprite();
	void Multicast_SetNormalSprite_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_DisableInput();
	void Client_DisableInput_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Client_EnableInput();
	void Client_EnableInput_Implementation();

public:
	virtual void OnDeath() override;

// Exp Section
protected:
	bool bIsCompReady;
	bool bIsControllerReady;
	void AttatchExpCompToBattleWidget();

// MoveSpeed Section
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetMoveSpeed();
	void Multicast_ResetMoveSpeed_Implementation();
};
