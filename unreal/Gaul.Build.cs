// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class Gaul : ModuleRules
{
	public Gaul(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});
		PrivateDependencyModuleNames.AddRange(new string[] {  "HTTP", "Json", "JsonUtilities" });     
   }
}
