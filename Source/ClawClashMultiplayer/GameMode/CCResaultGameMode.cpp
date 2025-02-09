// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClashMultiplayer/GameMode/CCResaultGameMode.h"
#include "ClawClashMultiplayer/GameInstance/CCGameInstance.h"
#include "ClawClashMultiplayer/GameState/CCResaultGameState.h"
#include "Http.h"
#include "ClawClashMultiplayer/PlayerState/CCTeamPlayerState.h"

void ACCResaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	UCCGameInstance* GameInstance = Cast<UCCGameInstance>(GetGameInstance());
	
	if (GameInstance)
	{
		GetGameState<ACCResaultGameState>()->SetWinnerTeam(GameInstance->GetWinnerTeam());
	}

    FString RedID = GameInstance->GetTeamPlayerID(EPlayerTeam::Red);
    FString BlueID = GameInstance->GetTeamPlayerID(EPlayerTeam::Blue);

    FString RedWinString;
    FString BlueWinString;
    
    switch (GameInstance->GetWinnerTeam())
    {
    case EPlayerTeam::None:
        RedWinString = TEXT("draw");
        BlueWinString = TEXT("draw");
        break;
    case EPlayerTeam::Red:
        RedWinString = TEXT("win");
        BlueWinString = TEXT("lose");
        break;
    case EPlayerTeam::Blue:
        RedWinString = TEXT("lose");
        BlueWinString = TEXT("win");
    }

    FString RedTeamScoreString = FString::FromInt(GameInstance->GetRedScore());
    FString BlueTeamScoreString = FString::FromInt(GameInstance->GetBlueScore());

    Server_SendGameResult(RedID, BlueID, TeamEnumToString(EPlayerTeam::Red).ToLower(), RedWinString, RedTeamScoreString, BlueTeamScoreString);
    Server_SendGameResult(BlueID, RedID, TeamEnumToString(EPlayerTeam::Blue).ToLower(), BlueWinString, RedTeamScoreString, BlueTeamScoreString);

	// UE_LOG(LogTemp, Log, TEXT("%d, %d, %s"), GameInstance->GetRedScore(), GameInstance->GetBlueScore(), WinnerTeamString);
}


void ACCResaultGameMode::Server_SendGameResult_Implementation(const FString& UserId, const FString& Opponent, const FString& MyTeam, const FString& WinStatus, const FString& RedScore, const FString& BlueScore)
{
    UE_LOG(LogTemp, Log, TEXT("Sending game result..."));

    // HTTP 모듈을 가져와 요청 생성
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    FString Url = FString::Printf(TEXT("http://ec2-13-125-230-55.ap-northeast-2.compute.amazonaws.com:5000/%s/gameRecord"), *UserId);
    Request->SetURL(Url);
    Request->SetVerb("POST");

    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField("versus", Opponent);
    JsonObject->SetStringField("myTeam", MyTeam);
    JsonObject->SetStringField("win", WinStatus);
    JsonObject->SetStringField("redScore", RedScore);
    JsonObject->SetStringField("blueScore", BlueScore);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);

    Request->OnProcessRequestComplete().BindUObject(this, &ACCResaultGameMode::OnGameResultResponseReceived);

    // 요청 실행
    Request->ProcessRequest();
}

void ACCResaultGameMode::OnGameResultResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseData = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Game result response: %s"), *ResponseData);

        // 서버에서 성공적인 응답을 받았는지 확인하고 로그 처리 또는 추가 처리를 수행할 수 있음
        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

        if (FJsonSerializer::Deserialize(Reader, JsonResponse) && JsonResponse.IsValid())
        {
            bool bSuccess = JsonResponse->GetBoolField("success"); // 서버 응답이 성공 여부를 포함한다고 가정
            if (bSuccess)
            {
                UE_LOG(LogTemp, Log, TEXT("Game result saved successfully!"));
            }
            else
            {
                FString ErrorMessage = JsonResponse->GetStringField("error");
                UE_LOG(LogTemp, Warning, TEXT("Failed to save game result: %s"), *ErrorMessage);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to send game result request"));
    }
}

FString ACCResaultGameMode::TeamEnumToString(EPlayerTeam Team)
{
    const UEnum* EnumPtr = StaticEnum<EPlayerTeam>();
    if (!EnumPtr)
    {
        return FString("Invalid");
    }

    return EnumPtr->GetNameStringByValue(static_cast<int64>(Team));
}

void ACCResaultGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (GameState)
    {
        if (GameState->PlayerArray.Num() == 1)
        {
            if (HasAuthority())
            {
                Cast<UCCGameInstance>(GetGameInstance())->ResetGameInstance();
                FString LevelName = TEXT("/Game/Maps/GameLobbyLevel");
                GetWorld()->ServerTravel(LevelName, true);
            }
        }
    }
}
