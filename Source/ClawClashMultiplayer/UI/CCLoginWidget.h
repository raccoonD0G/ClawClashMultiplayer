// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClashMultiplayer/UI/CCPopupWidget.h"
#include "CCLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASHMULTIPLAYER_API UCCLoginWidget : public UCCPopupWidget
{
	GENERATED_BODY()
	
public:
    void Init(class ACCHttpLogin* InHttpLogin);

protected:
	virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UEditableTextBox> IDTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UEditableTextBox> PWTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> LoginButton;

    UPROPERTY()
    TObjectPtr<class ACCHttpLogin> HttpLogin;

private:
    UFUNCTION()
    void OnLoginButtonClicked();
};
