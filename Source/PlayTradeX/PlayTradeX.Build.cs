// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class PlayTradeX : ModuleRules
{
    public PlayTradeX(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "DeveloperSettings"
            }
        );

        string PluginDirectory = Path.GetFullPath(
            Path.Combine(ModuleDirectory, "..", "..")
        );

        string NativeSDKDirectory = Path.Combine(
            PluginDirectory,
            "ThirdParty",
            "PlayTradeXSDK"
        );

        string IncludeDirectory = Path.Combine(
            NativeSDKDirectory,
            "Include"
        );

        PublicIncludePaths.Add(IncludeDirectory);

        PublicDefinitions.Add("PLAYTRADEX_SHARED");


        // =========================================================
        // Windows
        // =========================================================

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateDependencyModuleNames.Add("DesktopPlatform");

            string Win64Directory = Path.Combine(
                NativeSDKDirectory,
                "Win64"
            );

            string ImportLibrary = Path.Combine(
                Win64Directory,
                "PlayTradeXSDK.lib"
            );

            string RuntimeLibrary = Path.Combine(
                Win64Directory,
                "PlayTradeXSDK.dll"
            );

            PublicAdditionalLibraries.Add(ImportLibrary);

            RuntimeDependencies.Add(
                "$(BinaryOutputDir)/PlayTradeXSDK.dll",
                RuntimeLibrary
            );
        }


        // =========================================================
        // Android
        // =========================================================

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("Launch");

            string AndroidDirectory = Path.Combine(
                NativeSDKDirectory,
                "Android",
                "arm64-v8a"
            );

            string AndroidLibrary = Path.Combine(
                AndroidDirectory,
                "libPlayTradeXSDK.so"
            );

            PublicAdditionalLibraries.Add(
                AndroidLibrary
            );


            // =====================================================
            // PlayTradeX Android UPL
            // =====================================================

            string AndroidPlugin = Path.Combine(
                ModuleDirectory,
                "Android",
                "PlayTradeX_UPL.xml"
            );

            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                AndroidPlugin
            );
        }
    }
}