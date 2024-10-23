// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "ClawClashMultiplayer/Components/HealthComponent.h"

ACCPaperCharacter::ACCPaperCharacter() :Super()
{
	;
}

float ACCPaperCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause)
{
	UHealthComponent* HealthComponent = GetComponentByClass<UHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->GetDamaged(DamageAmount);
	}
	return 0.0f;
}
