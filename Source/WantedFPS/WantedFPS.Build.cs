// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WantedFPS : ModuleRules
{
	public WantedFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"WantedFPS",
			"WantedFPS/Variant_Horror",
			"WantedFPS/Variant_Horror/UI",
			"WantedFPS/Variant_Shooter",
			"WantedFPS/Variant_Shooter/AI",
			"WantedFPS/Variant_Shooter/UI",
			"WantedFPS/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
