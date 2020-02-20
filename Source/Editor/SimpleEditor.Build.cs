// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimpleEditor : ModuleRules
{
	public SimpleEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "AnimationModifiers",
            }
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "PropertyEditor",
                "InputCore",
                "UMG",
                "IntroTutorials",
                "AnimationModifiers",
                "LevelSequence",
                "UnrealEd",
            }
            );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
