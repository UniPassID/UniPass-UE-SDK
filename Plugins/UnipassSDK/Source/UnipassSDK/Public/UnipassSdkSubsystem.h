// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "UnipassSdkTypes.h"

#include "UnipassSdkSubsystem.generated.h"

class UUnipassWebBrowserWrapper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogin, const FUnipassLoginData&, LoginData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignMessage, const FString&, Signature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendTransaction, const FString&, TxHash);

UCLASS()
class UNIPASSSDK_API UUnipassSdkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUnipassSdkSubsystem();

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UFUNCTION(BlueprintCallable, Category = "Unipass | Init", meta = (WorldContext = "WorldContextObject"))
		void Init(
			UObject* WorldContextObject,
			TSubclassOf<UUserWidget> DefaultBrowserWidget,
			const EDevelopment InDevelopment = EDevelopment::testnet,
			const EChainType InChainType = EChainType::polygon,
			const ETheme InTheme = ETheme::light,
			const FString InAppName = "",
			const FString InAppIcon = ""
		);

	UFUNCTION(BlueprintCallable, Category = "Unipass | Login")
		void Login(const bool returnEmail = false);

	UFUNCTION(BlueprintCallable, Category = "Unipass | OnLogin")
		void OnLogin(const FUnipassLoginData& LoginData);

	UFUNCTION(BlueprintCallable, Category = "Unipass | SignMessage")
		void SignMessage(const FString Message = "");

	UFUNCTION(BlueprintCallable, Category = "Unipass | OnSignMessage")
		void OnSignMessage(const FString& Signature);

	UFUNCTION(BlueprintCallable, Category = "Unipass | SendTransaction")
		void SendTransaction(const FSendTransactionParamsData& TxData);

	UFUNCTION(BlueprintCallable, Category = "Unipass | OnSendTransaction")
		void OnSendTransaction(const FString& TxHash);

	UFUNCTION(BlueprintCallable, Category = "Unipass | ClearCache")
		void ClearCache();

public:
	UPROPERTY(BlueprintAssignable, Category = "Unipass | OnLoginEvent")
		FOnLogin OnLoginEvent;

	UPROPERTY(BlueprintAssignable, Category = "Unipass | OnSignMessageEvent")
		FOnSignMessage OnSignMessageEvent;

	UPROPERTY(BlueprintAssignable, Category = "Unipass | OnSendTransactionEvent")
		FOnSendTransaction OnSendTransactionEvent;

private:
	FString WalletURL;

	EDevelopment Development;
	FString AppName;
	FString AppIcon;
	EChainType ChainType;
	ETheme Theme;

	UUnipassWebBrowserWrapper* MyBrowser;

protected:
	/** Keeps state of user login. */
	FUnipassLoginData LoginDataCached;

private:
	UPROPERTY()
		TSubclassOf<UUserWidget> DefaultBrowserWidgetClass;
};
