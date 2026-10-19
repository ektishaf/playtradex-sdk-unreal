using UnrealBuildTool;
using System.IO;

public class PlayTradeXEditor : ModuleRules
{
    public PlayTradeXEditor(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "InputCore",
                "PropertyEditor",
                "DeveloperSettings",
                "ToolMenus",
                "Settings",
                "Projects",
                "Json",
                "PlayTradeX",
                "AssetRegistry",
                "AssetTools",
                "ContentBrowser"
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

        PrivateIncludePaths.Add(
            IncludeDirectory
        );

        PrivateDefinitions.Add(
            "PLAYTRADEX_SHARED"
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string Win64Directory = Path.Combine(
                NativeSDKDirectory,
                "Win64"
            );

            string ImportLibrary = Path.Combine(
                Win64Directory,
                "PlayTradeXSDK.lib"
            );

            PublicAdditionalLibraries.Add(
                ImportLibrary
            );
        }
    }
}