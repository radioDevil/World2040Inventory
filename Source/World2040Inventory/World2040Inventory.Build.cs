// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class World2040Inventory : ModuleRules
{
	public World2040Inventory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
