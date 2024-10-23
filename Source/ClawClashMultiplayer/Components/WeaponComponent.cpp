// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "ClawClashMultiplayer/Interfaces/CCAttacker.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/Character/CCPaperCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WeaponCenterTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WeaponCenterTimeline"));
	SetIsReplicatedByDefault(true);
	WeaponCenterTimeline->SetIsReplicated(true);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Attacker = TScriptInterface<ICCAttacker>(GetOwner());

	UpdateTimelineFloatTrack.BindDynamic(this, &UWeaponComponent::UpdateWeaponCenterRotation);

	if (WeaponRotateFloatCurve)
	{
		WeaponCenterTimeline->AddInterpFloat(WeaponRotateFloatCurve, UpdateTimelineFloatTrack);
	}

	WeaponCenterTimeline->SetLooping(false);

	OnTimelineEnd.BindDynamic(this, &UWeaponComponent::EndAttack);
	WeaponCenterTimeline->SetTimelineFinishedFunc(OnTimelineEnd);

	

	SetActive(false, true);
	SetVisibility(false, true);
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::DoAttack()
{
	SetVisibility(true, true);
	SetActive(true, true);

	SetRelativeScale3D(FVector(Attacker->GetAttackRangeMultiplier(), 1.0f, Attacker->GetAttackRangeMultiplier()));

	WeaponFlipbook->SetPlaybackPositionInFrames(1, true);
	WeaponFlipbook->Play();

	WeaponCenter->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	WeaponCenterTimeline->SetPlayRate(1.0f / GetWeaponFlipbookLengthInSeconds());
	WeaponCenterTimeline->PlayFromStart();
	if (GetOwner()->HasAuthority())
	{
		WeaponCollider->SetCollisionProfileName(TEXT("CCWeapon"));
	}
}

void UWeaponComponent::EndAttack()
{
	SetVisibility(false, true);
	SetActive(false, true);
	WeaponCenter->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	WeaponCollider->SetCollisionProfileName(TEXT("NoCollision"));
}


void UWeaponComponent::SetWeaponCollider(UCapsuleComponent* InWeaponCollider)
{
	WeaponCollider = InWeaponCollider;
	WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponent::OnOverlapBegin);
}

float UWeaponComponent::GetWeaponFlipbookLengthInSeconds() const
{
	if (WeaponFlipbook)
	{
		int32 NumFrames = WeaponFlipbook->GetFlipbook()->GetNumFrames();
		float FramesPerSecond = WeaponFlipbook->GetFlipbook()->GetFramesPerSecond();

		if (FramesPerSecond > 0)
		{
			return (float)NumFrames / FramesPerSecond;
		}
	}

	return 0.0f;
}

void UWeaponComponent::UpdateWeaponCenterRotation(float Output)
{
	WeaponCenter->SetRelativeRotation(FRotator(-Output, 0.0f, 0.0f));
	int32 CurrentFrame = WeaponFlipbook->GetPlaybackPositionInFrames();
	if (CurrentFrame == WeaponFlipbook->GetFlipbook()->GetNumFrames() - 1)
	{
		WeaponFlipbook->Stop();
	}
}

void UWeaponComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}

	float DamageAmount = Attacker->GetNormalAttackPower();
	DamageAmount *= Attacker->GetAttackPowerMultiplier();
	ACCPaperCharacter* Character = Cast<ACCPaperCharacter>(OtherActor);
	if (Character)
	{
		UGameplayStatics::ApplyDamage(Character, DamageAmount, nullptr, GetOwner(), nullptr);
	}
}
