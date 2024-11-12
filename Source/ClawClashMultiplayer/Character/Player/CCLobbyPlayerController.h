// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API ACCLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Ready Section
public:
	UFUNCTION(Server, Reliable)
	void Server_OnReadyButtonClicked();
	void Server_OnReadyButtonClicked_Implementation();

// Login Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCCPopupWidget> LoginWidgetClass;

	UPROPERTY()
	TObjectPtr<class UCCLoginWidget> LoginWidget;

	UPROPERTY(ReplicatedUsing = OnRep_HttpLogin)
	TObjectPtr<class ACCHttpLogin> HttpLogin;

	UFUNCTION()
	void OnRep_HttpLogin();

	UFUNCTION(Reliable, Client)
	void Client_CloseLoginWidget();
	void Client_CloseLoginWidget_Implementation();
};
