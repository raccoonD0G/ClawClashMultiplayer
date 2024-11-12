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

    // URL 설정
    FString Url = FString::Printf(TEXT("http://ec2-13-125-230-55.ap-northeast-2.compute.amazonaws.com:5000/%s/verify"), *UserId);
    Request->SetURL(Url);
    Request->SetVerb("POST");

    // JSON 형식의 본문 데이터 설정
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField("userpassword", UserPassword);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);

    UCCGameInstance* GameInstance = Cast<UCCGameInstance>(GetGameInstance());
    GameInstance->AddID(Team, UserId);

    // 응답 콜백 설정
    Request->OnProcessRequestComplete().BindUObject(this, &ACCHttpLogin::OnResponseReceived);

    // 요청 실행
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

        // 로그인 성공 여부를 판단할 수 있도록 JSON 파싱 수행 가능
        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

        if (FJsonSerializer::Deserialize(Reader, JsonResponse) && JsonResponse.IsValid())
        {
            bool bLoginSuccess = JsonResponse->GetBoolField("success"); // 예: JSON 응답에 success 필드가 있을 경우
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
