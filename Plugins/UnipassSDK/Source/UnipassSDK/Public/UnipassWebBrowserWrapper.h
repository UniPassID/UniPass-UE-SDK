// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnipassWebBrowserWrapper.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(FOnBrowserClosed, const bool);
DECLARE_DELEGATE(FOnPageLoaded);

UCLASS()
class UNIPASSSDK_API UUnipassWebBrowserWrapper : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Unipass | Browser")
		void LoadUrl(const FString& Url);

	UFUNCTION(BlueprintCallable, Category = "Unipass | Browser")
		void BrowserClosed(const bool FromUser);

	UFUNCTION(BlueprintCallable, Category = "Unipass | Browser")
		void PageLoaded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Unipass | Browser")
		void ExecuteJavascript(const FString& ScriptText);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Unipass | Browser")
		void ClearCache();

	FOnBrowserClosed OnBrowserClosed;
	FOnPageLoaded OnPageLoaded;
};
