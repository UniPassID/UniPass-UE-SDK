// Fill out your copyright notice in the Description page of Project Settings.


#include "UnipassSdkSubsystem.h"
#include "UnipassWebBrowserWrapper.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

UUnipassSdkSubsystem::UUnipassSdkSubsystem()
	: UGameInstanceSubsystem()
{
}

void UUnipassSdkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUnipassSdkSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UUnipassSdkSubsystem::Init(
	UObject* WorldContextObject,
	TSubclassOf<UUserWidget> DefaultBrowserWidget,
	const EDevelopment InDevelopment,
	const EChainType InChainType,
	const ETheme InTheme,
	const FString InAppName,
	const FString InAppIcon
)
{
	Development = InDevelopment;
	Theme = InTheme;
	AppName = InAppName;
	AppIcon = InAppIcon;
	ChainType = InChainType;

	this->DefaultBrowserWidgetClass = DefaultBrowserWidget;

	if (Development == EDevelopment::mainnet) 
		WalletURL = "https://wallet.unipass.id/";
	else
		WalletURL = "https://t.wallet.unipass.vip/";

	UE_LOG(LogTemp, Log, TEXT("Unipass subsystem initialized Url=%s"), *WalletURL);

	if (MyBrowser == nullptr || !MyBrowser->IsValidLowLevel() || !MyBrowser->GetIsEnabled())
	{
		MyBrowser = CreateWidget<UUnipassWebBrowserWrapper>(WorldContextObject->GetWorld(), DefaultBrowserWidget);
		MyBrowser->AddToViewport(100000);

		MyBrowser->SetVisibility(ESlateVisibility::Hidden);
	}

	MyBrowser->OnBrowserClosed.Unbind();
	MyBrowser->OnBrowserClosed.BindLambda([&](const bool bFromUser)
	{
		MyBrowser->SetVisibility(ESlateVisibility::Hidden);
		MyBrowser->LoadUrl("about:blank");
		UE_LOG(LogTemp, Log, TEXT("Unipass Browser Closed"));
	});
}

void UUnipassSdkSubsystem::Login(const bool returnEmail)
{
	if (MyBrowser == nullptr)
		return;

	MyBrowser->LoadUrl(WalletURL + "connect");
	MyBrowser->SetVisibility(ESlateVisibility::Visible);

	MyBrowser->OnPageLoaded.Unbind();
	MyBrowser->OnPageLoaded.BindLambda([&](const bool returnEmail_p)
	{
		TSharedPtr<FJsonObject> HandleMessageJson = MakeShareable(new FJsonObject());

		HandleMessageJson->SetStringField(TEXT("type"), "UP_LOGIN");

		TSharedPtr<FJsonObject> AppSettingDataJson = MakeShareable(new FJsonObject());
		AppSettingDataJson->SetStringField(TEXT("appName"), AppName);
		AppSettingDataJson->SetStringField(TEXT("appIcon"), AppIcon);

		FString Chain = *UEnum::GetDisplayValueAsText(ChainType).ToString();
		FString Theme_ = *UEnum::GetDisplayValueAsText(Theme).ToString();

		AppSettingDataJson->SetStringField(TEXT("chain"), Chain);
		AppSettingDataJson->SetStringField(TEXT("theme"), Theme_);
		AppSettingDataJson->SetBoolField(TEXT("returnEmail"), returnEmail_p);
		HandleMessageJson->SetObjectField(TEXT("appSetting"), AppSettingDataJson);

		FString ScriptContent;

		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ScriptContent);
		FJsonSerializer::Serialize(HandleMessageJson.ToSharedRef(), Writer);

		const FString Script = FString::Printf(TEXT("onConnectPageReady(`%s`)"), *ScriptContent);

		UE_LOG(LogTemp, Log, TEXT("Unipass Login Data: %s"), *Script);
		MyBrowser->ExecuteJavascript(Script);
	}, returnEmail);
}

void UUnipassSdkSubsystem::OnLogin(const FUnipassLoginData& LoginData)
{
	if (MyBrowser == nullptr)
		return;

	LoginDataCached = LoginData;
	OnLoginEvent.Broadcast(LoginData);
	MyBrowser->BrowserClosed(true);
	UE_LOG(LogTemp, Log, TEXT("Unipass OnLogin address:%s email:%s newborn:%s"), *LoginData.address, *LoginData.email, *LoginData.newborn);
}

void UUnipassSdkSubsystem::SignMessage(const FString Message)
{
	if (MyBrowser == nullptr && LoginDataCached.address == "")
		return;

	MyBrowser->LoadUrl(WalletURL + "sign-message");
	MyBrowser->SetVisibility(ESlateVisibility::Visible);

	MyBrowser->OnPageLoaded.Unbind();
	MyBrowser->OnPageLoaded.BindLambda([&](const FString Message_p)
	{
		TSharedPtr<FJsonObject> HandleMessageJson = MakeShareable(new FJsonObject());

		HandleMessageJson->SetStringField(TEXT("type"), "UP_SIGN_MESSAGE");

		TSharedPtr<FJsonObject> AppSettingDataJson = MakeShareable(new FJsonObject());
		AppSettingDataJson->SetStringField(TEXT("appName"), AppName);
		AppSettingDataJson->SetStringField(TEXT("appIcon"), AppIcon);

		FString Chain = *UEnum::GetDisplayValueAsText(ChainType).ToString();
		FString Theme_ = *UEnum::GetDisplayValueAsText(Theme).ToString();


		AppSettingDataJson->SetStringField(TEXT("chain"), Chain);
		AppSettingDataJson->SetStringField(TEXT("theme"), Theme_);
		HandleMessageJson->SetObjectField(TEXT("appSetting"), AppSettingDataJson);

		TSharedPtr<FJsonObject> PayloadDataJson = MakeShareable(new FJsonObject());
		PayloadDataJson->SetStringField(TEXT("from"), LoginDataCached.address);
		PayloadDataJson->SetStringField(TEXT("msg"), Message_p);
		HandleMessageJson->SetObjectField(TEXT("payload"), PayloadDataJson);

		FString ScriptContent;

		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ScriptContent);
		FJsonSerializer::Serialize(HandleMessageJson.ToSharedRef(), Writer);

		const FString Script = FString::Printf(TEXT("onSignMessagePageReady(`%s`)"), *ScriptContent);

		UE_LOG(LogTemp, Log, TEXT("Unipass Sign Message Data: %s"), *Script);
		MyBrowser->ExecuteJavascript(Script);
	}, Message);
}

void UUnipassSdkSubsystem::OnSignMessage(const FString& Signature)
{
	if (MyBrowser == nullptr)
		return;

	OnSignMessageEvent.Broadcast(Signature);
	MyBrowser->BrowserClosed(true);
	UE_LOG(LogTemp, Log, TEXT("Signature:%s"), *Signature);
}

void UUnipassSdkSubsystem::SendTransaction(const FSendTransactionParamsData& TxData)
{
	if (MyBrowser == nullptr && LoginDataCached.address == "")
		return;


	MyBrowser->LoadUrl(WalletURL + "send-transaction");
	MyBrowser->SetVisibility(ESlateVisibility::Visible);

	MyBrowser->OnPageLoaded.Unbind();
	MyBrowser->OnPageLoaded.BindLambda([&](const FSendTransactionParamsData TxData_p)
	{
		TSharedPtr<FJsonObject> HandleMessageJson = MakeShareable(new FJsonObject());

		HandleMessageJson->SetStringField(TEXT("type"), "UP_TRANSACTION");

		TSharedPtr<FJsonObject> AppSettingDataJson = MakeShareable(new FJsonObject());
		AppSettingDataJson->SetStringField(TEXT("appName"), AppName);
		AppSettingDataJson->SetStringField(TEXT("appIcon"), AppIcon);

		FString Chain = *UEnum::GetDisplayValueAsText(TxData_p.ChainType).ToString();
		FString Theme_ = *UEnum::GetDisplayValueAsText(Theme).ToString();

		AppSettingDataJson->SetStringField(TEXT("chain"), Chain);
		AppSettingDataJson->SetStringField(TEXT("theme"), Theme_);
		HandleMessageJson->SetObjectField(TEXT("appSetting"), AppSettingDataJson);

		TSharedPtr<FJsonObject> PayloadDataJson = MakeShareable(new FJsonObject());
		PayloadDataJson->SetStringField(TEXT("from"), LoginDataCached.address);
		PayloadDataJson->SetStringField(TEXT("to"), TxData_p.To);
		PayloadDataJson->SetStringField(TEXT("value"), TxData_p.Value);
		PayloadDataJson->SetStringField(TEXT("data"), TxData_p.Data);
		HandleMessageJson->SetObjectField(TEXT("payload"), PayloadDataJson);

		FString ScriptContent;

		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ScriptContent);
		FJsonSerializer::Serialize(HandleMessageJson.ToSharedRef(), Writer);

		const FString Script = FString::Printf(TEXT("onSendTransactionPageReady(`%s`)"), *ScriptContent);

		UE_LOG(LogTemp, Log, TEXT("Unipass Send Transaction Data: %s"), *Script);
		MyBrowser->ExecuteJavascript(Script);
	}, TxData);
}

void UUnipassSdkSubsystem::OnSendTransaction(const FString& TxHash)
{
	if (MyBrowser == nullptr)
		return;

	OnSendTransactionEvent.Broadcast(TxHash);
	MyBrowser->BrowserClosed(true);
	UE_LOG(LogTemp, Log, TEXT("TxHash:%s"), *TxHash);
}

void UUnipassSdkSubsystem::ClearCache()
{
	if (MyBrowser == nullptr)
		return;
	MyBrowser->ClearCache();
}