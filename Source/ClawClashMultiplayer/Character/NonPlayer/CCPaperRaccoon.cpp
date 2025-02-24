// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperRaccoon.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "ClawClashMultiplayer/Components/PlayerDetectorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClashMultiplayer/Components/DamageSphereComponent.h"
#include <ClawClashMultiplayer/Components/HealthBarWidgetComponent.h>
#include "ClawClashMultiplayer/Components/ExpComponent.h"

ACCPaperRaccoon::ACCPaperRaccoon() : Super()
{
	IsRightStart = false;
}

void ACCPaperRaccoon::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentState(ERaccoonState::Idle);
}

void ACCPaperRaccoon::FaceDirection(FVector Dir)
{
	Super::FaceDirection(Dir);
}

void ACCPaperRaccoon::SetCurrentState(ERaccoonState NewState)
{
	switch (NewState)
	{
	case ERaccoonState::Idle:
		GetSprite()->SetFlipbook(IdleAnimation);
		GetSprite()->Play();
		break;
	case ERaccoonState::Move:
		GetSprite()->SetFlipbook(MoveAnimation);
		GetSprite()->Play();
		break;
	case ERaccoonState::ReadyAttack:
		GetSprite()->SetFlipbook(ReadyAttackAnimation);
		GetSprite()->Play();
		break;
	case ERaccoonState::Attack:
		GetSprite()->SetFlipbook(AttackAnimation);
		GetSprite()->Play();
		break;
	}

	CurrentState = NewState;
}

void ACCPaperRaccoon::StartJump()
{
	SetCurrentState(ERaccoonState::ReadyAttack);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CCAttackSphere"));
}

float ACCPaperRaccoon::GetJumpTime()
{
	return ReadyAttackAnimation->GetNumFrames() / GetSprite()->GetFlipbook()->GetFramesPerSecond();
}

void ACCPaperRaccoon::EndJump()
{
}

void ACCPaperRaccoon::StartAttack()
{
	SetCurrentState(ERaccoonState::Attack);
	DamageSphereComponent = NewObject<UDamageSphereComponent>(this);

	if (DamageSphereComponent)
	{
		DamageSphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		DamageSphereComponent->SetRelativeLocation(FVector::ZeroVector);
		DamageSphereComponent->RegisterComponent();
	}
}

float ACCPaperRaccoon::GetAttackTime()
{
	return AttackAnimation->GetNumFrames() / GetSprite()->GetFlipbook()->GetFramesPerSecond();
}

void ACCPaperRaccoon::EndAttack()
{
	SetCurrentState(ERaccoonState::Idle);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CCNonPlayer"));

	if (DamageSphereComponent)
	{
		DamageSphereComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageSphereComponent->DestroyComponent();
		DamageSphereComponent = nullptr;
	}
}

void ACCPaperRaccoon::StartMove()
{
	Super::StartMove();
	SetCurrentState(ERaccoonState::Move);
}

void ACCPaperRaccoon::EndMove()
{
	Super::EndMove();
	SetCurrentState(ERaccoonState::Idle);
}

void ACCPaperRaccoon::OnDeath(AActor* Destroyer)
{
	UExpComponent* ExpComponent = Destroyer->GetComponentByClass<UExpComponent>();
	if (ExpComponent)
	{
		ExpComponent->AddExp(700);
	}
	Destroy();
}
