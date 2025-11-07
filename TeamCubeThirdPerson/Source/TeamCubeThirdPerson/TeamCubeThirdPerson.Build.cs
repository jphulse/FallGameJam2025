// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TeamCubeThirdPerson : ModuleRules
{
	public TeamCubeThirdPerson(ReadOnlyTargetRules Target) : base(Target)
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
			"TeamCubeThirdPerson",
			"TeamCubeThirdPerson/Variant_Platforming",
			"TeamCubeThirdPerson/Variant_Platforming/Animation",
			"TeamCubeThirdPerson/Variant_Combat",
			"TeamCubeThirdPerson/Variant_Combat/AI",
			"TeamCubeThirdPerson/Variant_Combat/Animation",
			"TeamCubeThirdPerson/Variant_Combat/Gameplay",
			"TeamCubeThirdPerson/Variant_Combat/Interfaces",
			"TeamCubeThirdPerson/Variant_Combat/UI",
			"TeamCubeThirdPerson/Variant_SideScrolling",
			"TeamCubeThirdPerson/Variant_SideScrolling/AI",
			"TeamCubeThirdPerson/Variant_SideScrolling/Gameplay",
			"TeamCubeThirdPerson/Variant_SideScrolling/Interfaces",
			"TeamCubeThirdPerson/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
