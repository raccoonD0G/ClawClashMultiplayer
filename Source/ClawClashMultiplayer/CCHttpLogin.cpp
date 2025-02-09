// Fill out your copyright notice in the Description page of Project Settings.


#include "CCHttpLogin.h"
#include "Http.h"
#include <Kismet/GameplayStatics.h>
#include "GameMode/CCLobbyGameMode.h"
#include "ClawClashMultiplayer/Character/Player/CCPlayerController.h"
#include "PlayerState/CCTeamPlayerState.h"
#include "GameInstance/CCGameInstance.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"
#include "Managers/UIManager/CCUIManager.h"
#include "ClawClashMultiplayer/UI/CCLoginWidget.h"

// Sets default values
ACCHttpLogin::ACCHttpLogin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    }
}

void ACCHttpLogin::Server_SendLoginRequest_Implementation(const FString& InUserId, const FString& UserPassword, EPlayerTeam InTeam)
{
    UE_LOG(LogTemp, Log, TEXT("Start"));

    UserId = InUserId;
    Team = InTeam;

    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    FString Url = FString::Printf(TEXT("http://ec2-13-125-230-55.ap-northeast-2.compute.amazonaws.com:5000/%s/verify"), *UserId);
    Request->SetURL(Url);
    Request->SetVerb("POST");

    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField("userpassword", UserPassword);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);

    UCCGameInstance* GameInstance = Cast<UCCGameInstance>(GetGameInstance());
    GameInstance->AddID(Team, UserId);

    Request->OnProcessRequestComplete().BindUObject(this, &ACCHttpLogin::OnResponseReceived);

    Request->ProcessRequest();
}

void ACCHttpLogin::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!HasAuthority())
    {
        return;
    }

    if (bWasSuccessful && Response.IsValid())
    {
        // 서버 응답을 로그로 출력
        FString ResponseData = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseData);

        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

        if (FJsonSerializer::Deserialize(Reader, JsonResponse) && JsonResponse.IsValid())
        {
            bool bLoginSuccess = JsonResponse->GetBoolField("success");
            if (bLoginSuccess)
            {
                UE_LOG(LogTemp, Log, TEXT("Login successful!"));
                OnLoginSuccessEvent.Broadcast();
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Login failed."));
                UCCGameInstance* GameInstance = Cast<UCCGameInstance>(GetGameInstance());
                GameInstance->RemoveID(Team);
                OnLoginFailEvent.Broadcast();
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Request failed"));
    }
}
