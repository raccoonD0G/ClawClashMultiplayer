// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTree.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"

// Sets default values
ACCTree::ACCTree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxOccupation = 100;
	RedOccupation = 50;
	OccupySpeend = 10.0f;
	bReplicates = true;
}

void ACCTree::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCTree, RedOccupation);
}

// Called when the game starts or when spawned
void ACCTree::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		if (BoxComponent)
		{
			BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACCTree::OnOverlapBegin);
			BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACCTree::OnOverlapEnd);
		}
	}

	OnPercentChanged.Broadcast(GetRedOccupationPercent());
}

void ACCTree::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxComponent = GetComponentByClass<UBoxComponent>();
}

// Called every frame
void ACCTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (BluePlayer && !RedPlayer)
		{
			SetRedOccupation(RedOccupation - DeltaTime * OccupySpeend);
		}
		else if (RedPlayer && !BluePlayer)
		{
			SetRedOccupation(RedOccupation + DeltaTime * OccupySpeend);
		}

		if (RedOccupation < 0.01f)
		{
			SetCurrentState(TreeState::BlueOccupied);
		}
		else if (RedOccupation > MaxOccupation - 0.01f)
		{
			SetCurrentState(TreeState::RedOccupied);
		}
		else
		{
			SetCurrentState(TreeState::Neutral);
		}
	}
	
}

void ACCTree::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
		if (Player)
		{
			ACCTeamPlayerState* TeamPlayerState = Cast<ACCTeamPlayerState>(Player->GetController()->PlayerState);
			if (TeamPlayerState)
			{
				PlayerTeam Team = TeamPlayerState->GetTeam();
				switch (Team)
				{
				case PlayerTeam::Blue:
					BluePlayer = Player;
					break;
				case PlayerTeam::Red:
					RedPlayer = Player;
					break;
				default:
					break;
				}
			}
		}
	}
}

void ACCTree::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
		if (Player)
		{
			ACCTeamPlayerState* TeamPlayerState = Cast<ACCTeamPlayerState>(Player->GetController()->PlayerState);
			if (TeamPlayerState)
			{
				PlayerTeam Team = TeamPlayerState->GetTeam();
				switch (Team)
				{
				case PlayerTeam::Blue:
					BluePlayer = nullptr;
					break;
				case PlayerTeam::Red:
					RedPlayer = nullptr;
					break;
				default:
					break;
				}
			}
		}
	}
}

void ACCTree::SetCurrentState(TreeState InState)
{
	switch (InState)
	{
	case TreeState::Neutral:
		break;
	case TreeState::RedOccupied:
		break;
	case TreeState::BlueOccupied:
		break;
	default:
		break;
	}
	CurrentState = InState;
}

void ACCTree::SetRedOccupation(float InRedOccupation)
{
	RedOccupation = FMathf::Clamp(InRedOccupation, 0.0f, MaxOccupation);
	OnPercentChanged.Broadcast(GetRedOccupationPercent());
}

void ACCTree::OnRep_RedOccupation()
{
	OnPercentChanged.Broadcast(GetRedOccupationPercent());
}

