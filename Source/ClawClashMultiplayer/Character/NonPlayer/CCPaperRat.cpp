// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperRat.h"
#include "ClawClashMultiplayer/Managers/CCGameManager.h"
#include "ClawClashMultiplayer/Managers/SpawnManager/CCSpawnManager.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"
#include "ClawClashMultiplayer/Components/ExpComponent.h"

ACCPaperRat::ACCPaperRat() : Super()
{
	IsRightStart = true;
	ExpAmount = 100;
}

void ACCPaperRat::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent = GetComponentByClass<USphereComponent>();

	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACCPaperRat::OnOverlapBegin);
	}
	
	int32 Index = FMath::RandRange(0, UCCSpawnManager::GetInstance()->GetRatSpriteArr().Num() - 1);
	GetSprite()->SetFlipbook(UCCSpawnManager::GetInstance()->GetRatSpriteArr()[Index]);
}

void ACCPaperRat::StartMove()
{
	Super::StartMove();
}

void ACCPaperRat::EndMove()
{
	Super::EndMove();
}

void ACCPaperRat::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (OtherActor && OtherActor != this)
		{
			ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
			if (Player)
			{
				UExpComponent* ExpComponent = Player->GetComponentByClass<UExpComponent>();
				if (ExpComponent)
				{
					ExpComponent->AddExp(ExpAmount);
				}
				Destroy();
			}
		}
	}
}