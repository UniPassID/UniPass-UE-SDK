// Copyright Epic Games, Inc. All Rights Reserved.

#include "unipass_ue_sdkGameMode.h"
#include "unipass_ue_sdkCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aunipass_ue_sdkGameMode::Aunipass_ue_sdkGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
