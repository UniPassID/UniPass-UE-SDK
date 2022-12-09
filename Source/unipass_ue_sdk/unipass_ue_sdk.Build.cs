// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class unipass_ue_sdk : ModuleRules
{
	public unipass_ue_sdk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
