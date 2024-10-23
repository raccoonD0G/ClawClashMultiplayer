// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerSpawner.h"

// Sets default values
ACCPlayerSpawner::ACCPlayerSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACCPlayerSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}