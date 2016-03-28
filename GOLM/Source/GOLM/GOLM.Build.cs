// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GOLM : ModuleRules
{
	public GOLM(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "PhysX", "APEX" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"   });
		
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		
		if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		{
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
		}
	}
}
