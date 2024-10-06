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

	BoxComponent = GetComponentByClass<UBoxComponent>();
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
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACCTree::OnOverlapBegin);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACCTree::OnOverlapEnd);
	}
}

// Called every frame
void ACCTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BluePlayer && !RedPlayer)
	{
		RedOccupation -= DeltaTime;
		OnPercentChanged.Broadcast(GetRedOccupationPercent());
	}
	else if (RedPlayer && !BluePlayer)
	{
		RedOccupation += DeltaTime;
		OnPercentChanged.Broadcast(GetRedOccupationPercent());
	}
}

void ACCTree::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		APawn* Pawn = Cast<APawn>(OtherActor);
		if (Pawn)
		{
			ACCTeamPlayerState* TeamPlayerState = Cast<ACCTeamPlayerState>(Pawn->GetController()->PlayerState);
			if (TeamPlayerState)
			{
				PlayerTeam Team = TeamPlayerState->GetTeam();
				switch (Team)
				{
				case PlayerTeam::Blue:
					BluePlayer = Cast<ACCPaperPlayer>(OtherActor);
					break;
				case PlayerTeam::Red:
					RedPlayer = Cast<ACCPaperPlayer>(OtherActor);
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
		APawn* Pawn = Cast<APawn>(OtherActor);
		if (Pawn)
		{
			ACCTeamPlayerState* TeamPlayerState = Cast<ACCTeamPlayerState>(Pawn->GetController()->PlayerState);
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

void ACCTree::OnRep_RedOccupation()
{
}

