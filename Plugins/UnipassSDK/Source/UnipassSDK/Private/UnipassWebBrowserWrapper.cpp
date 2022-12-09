// Fill out your copyright notice in the Description page of Project Settings.


#include "UnipassWebBrowserWrapper.h"

void UUnipassWebBrowserWrapper::BrowserClosed(const bool FromUser)
{
	OnBrowserClosed.ExecuteIfBound(FromUser);
}

void UUnipassWebBrowserWrapper::PageLoaded()
{
	OnPageLoaded.ExecuteIfBound();
}