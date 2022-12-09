#pragma once

#include "UnipassSdkTypes.generated.h"

UENUM(BlueprintType)
enum class EDevelopment : uint8
{
	mainnet UMETA(DisplayName = "mainnet"),
	testnet UMETA(DisplayName = "testnet"),
};

UENUM(BlueprintType)
enum class EChainType : uint8
{
	polygon UMETA(DisplayName = "polygon"),
	eth UMETA(DisplayName = "eth"),
	bsc UMETA(DisplayName = "bsc"),
	rangers UMETA(DisplayName = "rangers"),
};

UENUM(BlueprintType)
enum class ETheme : uint8
{
	dark UMETA(DisplayName = "dark"),
	light UMETA(DisplayName = "light"),
};

USTRUCT(BlueprintType)
struct FUnipassLoginData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Login Data")
		FString address;

	UPROPERTY(BlueprintReadWrite, Category = "Login Data")
		FString email;

	UPROPERTY(BlueprintReadWrite, Category = "Login Data")
		FString newborn;

	FUnipassLoginData()
	{
	}
};

USTRUCT(BlueprintType)
struct FSendTransactionParamsData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SendTransactionParamsData Data")
		FString To;

	UPROPERTY(BlueprintReadWrite, Category = "SendTransactionParamsData Data")
		FString Value;

	UPROPERTY(BlueprintReadWrite, Category = "SendTransactionParamsData Data")
		FString Data;

	UPROPERTY(BlueprintReadWrite, Category = "SendTransactionParamsData Data")
		EChainType ChainType;


	FSendTransactionParamsData()
	{
	}
};
