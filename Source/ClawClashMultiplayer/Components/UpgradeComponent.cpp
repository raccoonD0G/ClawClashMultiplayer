// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/UpgradeComponent.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "ClawClashMultiplayer/Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCUpgradeWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UUpgradeComponent::UUpgradeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MoveSpeedLevel = 1;
	JumpPowerLevel = 1;
	AttackPowerLevel = 1;
	AttackRangeLevel = 1;
	ExpLevel = 1;
	OccupySpeedLevel = 1;

	MoveSpeedMultiplier = 1.0f;
	JumpPowerMultiplier = 1.0f;
	AttackPowerMultiplier = 1.0f;
	AttackRangeMultiplier = 1.0f;
	ExpMultiplier = 1.0f;
	OccupySpeedMultiplier = 1.0f;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	ExpComponent = GetOwner()->GetComponentByClass<UExpComponent>();
	ExpComponent->AddExpMultiplier([this]() -> float
		{
			return ExpMultiplier;
		});
	
	Player = Cast<ACCPaperPlayer>(GetOwner());

	Player->AddJumpPowerMultipliers([this]() -> float
		{
			return JumpPowerMultiplier;
		});

	Player->AddMoveSpeedMultipliers([this]() -> float
		{
			return MoveSpeedMultiplier;
		});

	Player->AddAttackPowerMultipliers([this]() -> float
		{
			return AttackPowerMultiplier;
		});

	Player->AddAttackRangeMultipliers([this]() -> float
		{
			return AttackRangeMultiplier;
		});

	Player->AddOccupySpeedMultipliers([this]() -> float
		{
			return OccupySpeedMultiplier;
		});

	ExpComponent->OnLevelChange.AddDynamic(this, &UUpgradeComponent::ShowUpgradePopup);

	if (!GetOwner()->HasAuthority())
	{
		UpgradeMap.Add(UpgradeType::MoveSpeed, [this]()
			{
				Server_SetMoveSpeedLevel(GetMoveSpeedLevel() + 1);
			});

		UpgradeMap.Add(UpgradeType::JumpPower, [this]()
			{
				Server_SetMoveSpeedLevel(GetJumpPowerLevel() + 1);
			});

		UpgradeMap.Add(UpgradeType::AttackPower, [this]()
			{
				Server_SetAttackPowerLevel(GetAttackPowerLevel() + 1);
			});

		UpgradeMap.Add(UpgradeType::AttackRange, [this]()
			{
				Server_SetAttackRangeLevel(GetAttackRangeLevel() + 1);
			});

		UpgradeMap.Add(UpgradeType::Exp, [this]()
			{
				Server_SetExpLevel(GetExpLevel() + 1);
			});

		UpgradeMap.Add(UpgradeType::OccupySpeed, [this]()
			{
				Server_SetOccupySpeedLevel(GetOccupySpeedLevel() + 1);
			});
	}
}

void UUpgradeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUpgradeComponent, MoveSpeedLevel);
	DOREPLIFETIME(UUpgradeComponent, JumpPowerLevel);
	DOREPLIFETIME(UUpgradeComponent, AttackPowerLevel);
	DOREPLIFETIME(UUpgradeComponent, AttackRangeLevel);
	DOREPLIFETIME(UUpgradeComponent, ExpLevel);
	DOREPLIFETIME(UUpgradeComponent, OccupySpeedLevel);
	DOREPLIFETIME(UUpgradeComponent, MoveSpeedMultiplier);
	DOREPLIFETIME(UUpgradeComponent, JumpPowerMultiplier);
	DOREPLIFETIME(UUpgradeComponent, AttackPowerMultiplier);
	DOREPLIFETIME(UUpgradeComponent, AttackRangeMultiplier);
	DOREPLIFETIME(UUpgradeComponent, ExpMultiplier);
	DOREPLIFETIME(UUpgradeComponent, OccupySpeedMultiplier);
}

void UUpgradeComponent::Server_SetMoveSpeedLevel_Implementation(int32 InMoveSpeedLevel)
{
	MoveSpeedLevel = InMoveSpeedLevel;
	SetMoveSpeedMultiplier(1.0f + MoveSpeedLevel * 0.2f);
}

void UUpgradeComponent::Server_SetJumpPowerLevel_Implementation(int32 InJumpPowerLevel)
{
	JumpPowerLevel = InJumpPowerLevel;
	SetJumpPowerMultiplier(1.0f + JumpPowerLevel * 0.2f);
}

void UUpgradeComponent::Server_SetAttackPowerLevel_Implementation(int32 InAttackPowerLevel)
{
	AttackPowerLevel = InAttackPowerLevel;
	SetAttackPowerMultiplier(1.0f + AttackPowerLevel * 0.2f);
}

void UUpgradeComponent::Server_SetAttackRangeLevel_Implementation(int32 InAttackRangeLevel)
{
	AttackRangeLevel = InAttackRangeLevel;
	SetAttackRangeMultiplier(1.0f + AttackRangeLevel * 0.2f);
}

void UUpgradeComponent::Server_SetExpLevel_Implementation(int32 InExpLevel)
{
	ExpLevel = InExpLevel;
	SetExpMultiplier(1.0f + ExpLevel + 0.2f);
}

void UUpgradeComponent::Server_SetOccupySpeedLevel_Implementation(int32 InOccupySpeedLevel)
{
	OccupySpeedLevel = InOccupySpeedLevel;
	SetOccupySpeedMultiplier(1.0f + OccupySpeedLevel * 0.2f);
}

void UUpgradeComponent::ShowUpgradePopup(int32 NewLevel)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn->IsLocallyControlled())
	{
		UCCPopupWidget* Popup = UCCUIManager::GetInstance()->AddPopupWidget(GetWorld(), UpgradePopup);

		if (!Popup)
		{
			return;
		}

		UCCUpgradeWidget* Upgrade = Cast<UCCUpgradeWidget>(Popup);

		if (!Upgrade)
		{
			return;
		}

		Upgrade->Init(this);
	}
}

TFunction<void()>* UUpgradeComponent::GetUpgradeFunc(UpgradeType InUpgradeType)
{
	return UpgradeMap.Find(InUpgradeType);
}
