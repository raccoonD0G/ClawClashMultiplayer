// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/Components/DamageSphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClashMultiplayer/Character/Player/CCPaperPlayer.h"

UDamageSphereComponent::UDamageSphereComponent()
{
    DamageAmount = 10.0f;

    OnComponentBeginOverlap.AddDynamic(this, &UDamageSphereComponent::OnOverlapBegin);

    SetCollisionProfileName(TEXT("CCAttackSphere"));
}

void UDamageSphereComponent::BeginPlay()
{
    Super::BeginPlay();

    SetSphereRadius(DamageRadius);
}

void UDamageSphereComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != GetOwner())
    {
        ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
        if (Player)
        {
            UGameplayStatics::ApplyDamage(Player, DamageAmount, nullptr, GetOwner(), nullptr);
        }
    }
}


