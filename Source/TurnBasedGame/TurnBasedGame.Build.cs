// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TurnBasedGame : ModuleRules
{
	public TurnBasedGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule" });
    }
}
