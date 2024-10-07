// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClashMultiplayer/Managers/LayerManager/CCLayerManager.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/Components/HealthComponent.h"


ACCPaperPlayer::ACCPaperPlayer() : Super()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovement
	GetCharacterMovement()->MaxWalkSpeed = 5000.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 10000.f;
	GetCharacterMovement()->AirControl = 100.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 4000.f;
	GetCharacterMovement()->GravityScale = 8.0f; 
	GetCharacterMovement()->MaxAcceleration = 10000;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	MaxJumpForce = 10000.0f;

	// Capsule
	GetCapsuleComponent()->SetCapsuleRadius(200.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(200.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CCPlayer"));
	GetCapsuleComponent()->BodyInstance.bLockYTranslation = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 7000.0f;
	CameraBoom->SocketOffset = FVector(7000.0f, 7000.0f, 0.f);
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetUsingAbsoluteScale(true);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	FollowCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	FollowCamera->PostProcessSettings.MotionBlurAmount = 0.0f;
	FollowCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	FollowCamera->OrthoWidth = 12000.0;

	if (IdleAnimation)
	{
		GetSprite()->SetFlipbook(IdleAnimation);
	}

	GetSprite()->SetIsReplicated(true);

	FVector NewLocation = GetActorLocation();
	NewLocation.Y = UCCLayerManager::GetPlayerY();
	SetActorLocation(NewLocation);

	bReplicates = true;
	GetSprite()->SetIsReplicated(true);
	SetReplicateMovement(true);
}

void ACCPaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACCPaperPlayer::LeftRightMove);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACCPaperPlayer::Server_StartReadyJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACCPaperPlayer::Server_ReleaseJump);
}

void ACCPaperPlayer::BeginPlay()
{
	Super::BeginPlay();

	Server_SetCurrentState(EPlayerState::Idle);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	if (PlayerController && IsLocallyControlled())
	{
		PlayerController->SetViewTarget(this);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

void ACCPaperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		switch (CurrentState)
		{
		case EPlayerState::Idle:
			UpdateIdle();
			break;

		case EPlayerState::ReadyJump:
			UpdateReadyJump(DeltaTime);
			break;

		case EPlayerState::KeepReadyJump:
			UpdateKeepReadyJump(DeltaTime);
			break;

		case EPlayerState::Jump:
			UpdateJump();
			break;

		case EPlayerState::Move:
			UpdateMove();
			break;

		case EPlayerState::Land:
			UpdateLand();
			break;

		case EPlayerState::Falling:
			UpdateFalling();
			break;

		case EPlayerState::KeepFalling:
			UpdateKeepFalling();
			break;

		default:
			break;
		}
	}
	
}

void ACCPaperPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HealthComponent = GetComponentByClass<UHealthComponent>();
}

void ACCPaperPlayer::UpdateIdle()
{
	float Speed = GetVelocity().Size();
	if (GetCharacterMovement()->IsMovingOnGround() && Speed > PlayerIdleThreshold)
	{
		Server_SetCurrentState(EPlayerState::Move);
	}

	if (GetCharacterMovement()->IsFalling())
	{
		Server_SetCurrentState(EPlayerState::Falling);
	}
}

void ACCPaperPlayer::UpdateMove()
{
	float Speed = GetVelocity().Size();
	if (GetCharacterMovement()->IsMovingOnGround() && Speed <= PlayerIdleThreshold)
	{
		Server_SetCurrentState(EPlayerState::Idle);
	}

	if (GetCharacterMovement()->IsFalling())
	{
		Server_SetCurrentState(EPlayerState::Falling);
	}
}

void ACCPaperPlayer::UpdateReadyJump(float DeltaTime)
{
	JumpReadyTime += DeltaTime;
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (CurrentFrame == GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		Server_SetCurrentState(EPlayerState::KeepReadyJump);
	}
}

void ACCPaperPlayer::UpdateKeepReadyJump(float DeltaTime)
{
	JumpReadyTime += DeltaTime;
}

void ACCPaperPlayer::UpdateJump()
{
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (CurrentFrame > 3)
	{
		Server_SetCurrentState(EPlayerState::Falling);
	}
}

void ACCPaperPlayer::UpdateLand()
{
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (CurrentFrame == GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		Server_SetCurrentState(EPlayerState::Idle);
	}
}

void ACCPaperPlayer::UpdateFalling()
{
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Server_SetCurrentState(EPlayerState::Land);
	}
	else if (CurrentFrame == GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		Server_SetCurrentState(EPlayerState::KeepFalling);
	}
}

void ACCPaperPlayer::UpdateKeepFalling()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Server_SetCurrentState(EPlayerState::Land);
	}
}

void ACCPaperPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCPaperPlayer, ServerPlayerYaw);
	DOREPLIFETIME(ACCPaperPlayer, CurrentState);
}

void ACCPaperPlayer::OnRep_CurrentState()
{
	SetAnimation();
}

void ACCPaperPlayer::OnRep_ServerPlayerYaw()
{
	FRotator Rotator = GetActorRotation();
	Rotator.Yaw = ServerPlayerYaw;
	GetSprite()->SetWorldRotation(Rotator);
}

void ACCPaperPlayer::Server_SetCurrentState_Implementation(EPlayerState NewState)
{
	CurrentState = NewState;
	SetAnimation();
}

void ACCPaperPlayer::SetAnimation()
{
	switch (CurrentState)
	{
	case EPlayerState::Idle:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(IdleAnimation);
		break;

	case EPlayerState::ReadyJump:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(ReadyJumpAnimation);
		break;

	case EPlayerState::KeepReadyJump:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(KeepReadyJumpAnimation);
		break;

	case EPlayerState::Jump:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(JumpAnimation);
		break;

	case EPlayerState::Move:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(MoveAnimation);
		break;

	case EPlayerState::Land:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(LandAnimation);
		break;

	case EPlayerState::Falling:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(FallingAnimation);
		break;

	case EPlayerState::KeepFalling:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(KeepFallingAnimation);
		break;

	default:
		break;
	}
}

void ACCPaperPlayer::LeftRightMove(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	
	if (CurrentState == EPlayerState::Jump || CurrentState == EPlayerState::ReadyJump || CurrentState == EPlayerState::KeepReadyJump) return;

	AddMovementInput(FVector(1.f, 0.f, 0.f), InputVector.X * 1.0f);

	Server_LeftRightMove(InputVector);
}

void ACCPaperPlayer::Server_LeftRightMove_Implementation(const FVector2D InputVector)
{
	FRotator Rotator = GetActorRotation();

	if (InputVector.X < 0)
	{
		Rotator.Yaw = 180.0f;
	}
	else if (InputVector.X > 0)
	{
		Rotator.Yaw = 0.0f;
	}

	ServerPlayerYaw = Rotator.Yaw;
}

void ACCPaperPlayer::Server_StartReadyJump_Implementation()
{
	if (GetCharacterMovement()->IsFalling() == false)
	{
		Server_SetCurrentState(EPlayerState::ReadyJump);
	}
}

void ACCPaperPlayer::Server_ReleaseJump_Implementation()
{
	if (CurrentState == EPlayerState::ReadyJump || CurrentState == EPlayerState::KeepReadyJump)
	{
		Server_SetCurrentState(EPlayerState::Jump);
		float JumpForce = FMath::Clamp(JumpReadyTime / MaxReadyTime, 0.0f, 1.0f) * MaxJumpForce;
		LaunchCharacter(FVector(0, 0, JumpForce), false, true);
		JumpReadyTime = 0.0f;
	}
}

float ACCPaperPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCause);
	if (HealthComponent)
	{
		HealthComponent->GetDamaged(DamageAmount);
	}
	return 0.0f;
}
