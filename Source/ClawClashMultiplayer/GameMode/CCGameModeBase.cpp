// Fill out your copyright notice in the Description page of Project Settings.


#include "CCGameModeBase.h"

ACCGameModeBase::ACCGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Blueprint/Character/Player/BP_CCPaperPlayer.BP_CCPaperPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ClawClashMultiplayer.CCPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	bUseSeamlessTravel = true;
}
