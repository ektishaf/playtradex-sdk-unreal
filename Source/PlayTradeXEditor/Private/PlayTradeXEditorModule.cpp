#include "PlayTradeXEditorModule.h"

#include "PlayTradeXAboutWindow.h"
#include "PlayTradeXAbiConverter.h"
#include "PlayTradeXSettings.h"
#include "PlayTradeXSettingsCustomization.h"

#include "ISettingsModule.h"

#include "Modules/ModuleManager.h"

#include "PropertyEditorModule.h"

#include "ToolMenus.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "Interfaces/IPluginManager.h"

#include "Misc/MessageDialog.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"

#include "FileHelpers.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "Interfaces/IPluginManager.h"

#include "Misc/MessageDialog.h"
#include "Misc/PackageName.h"

#include "ObjectTools.h"

#include "UObject/Package.h"
#include "UObject/SoftObjectPath.h"

#include "FileHelpers.h"


#define LOCTEXT_NAMESPACE "FPlayTradeXEditorModule"


// ============================================================
// Startup
// ============================================================

void FPlayTradeXEditorModule::StartupModule()
{
    // ========================================================
    // Register PlayTradeX Project Settings customization
    // ========================================================

    FPropertyEditorModule& PropertyEditorModule =
        FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
            TEXT("PropertyEditor")
        );


    PropertyEditorModule.RegisterCustomClassLayout(
        UPlayTradeXSettings::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(
            &FPlayTradeXSettingsCustomization::MakeInstance
        )
    );


    PropertyEditorModule.NotifyCustomizationModuleChanged();


    // ========================================================
    // Register PlayTradeX editor menu
    // ========================================================

    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(
            this,
            &FPlayTradeXEditorModule::RegisterMenus
        )
    );
}


// ============================================================
// Shutdown
// ============================================================

void FPlayTradeXEditorModule::ShutdownModule()
{
    // ========================================================
    // Unregister menus
    // ========================================================

    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::UnregisterOwner(
            this
        );
    }


    UToolMenus::UnRegisterStartupCallback(
        this
    );


    // ========================================================
    // Unregister Project Settings customization
    // ========================================================

    if (
        FModuleManager::Get().IsModuleLoaded(
            TEXT("PropertyEditor")
        )
        )
    {
        FPropertyEditorModule& PropertyEditorModule =
            FModuleManager::GetModuleChecked<FPropertyEditorModule>(
                TEXT("PropertyEditor")
            );


        PropertyEditorModule.UnregisterCustomClassLayout(
            UPlayTradeXSettings::StaticClass()->GetFName()
        );


        PropertyEditorModule.NotifyCustomizationModuleChanged();
    }
}


// ============================================================
// Register Menus
// ============================================================

void FPlayTradeXEditorModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(
        this
    );


    RegisterMainMenu();
}


// ============================================================
// Register Main Menu
// ============================================================

void FPlayTradeXEditorModule::RegisterMainMenu()
{
    UToolMenu* MainMenu =
        UToolMenus::Get()->ExtendMenu(
            TEXT("LevelEditor.MainMenu")
        );


    if (MainMenu == nullptr)
    {
        return;
    }


    FToolMenuSection& Section =
        MainMenu->FindOrAddSection(
            TEXT("PlayTradeX")
        );


    Section.AddSubMenu(
        TEXT("PlayTradeX.MainMenu"),
        LOCTEXT(
            "PlayTradeXMenuLabel",
            "PlayTradeX"
        ),
        LOCTEXT(
            "PlayTradeXMenuTooltip",
            "PlayTradeX SDK tools and settings"
        ),
        FNewToolMenuDelegate::CreateRaw(
            this,
            &FPlayTradeXEditorModule::PopulatePlayTradeXMenu
        ),
        false,
        FSlateIcon()
    );
}


// ============================================================
// Populate PlayTradeX Menu
// ============================================================

void FPlayTradeXEditorModule::PopulatePlayTradeXMenu(
    UToolMenu* Menu
)
{
    if (Menu == nullptr)
    {
        return;
    }


    // ========================================================
    // PlayTradeX
    // ========================================================

    FToolMenuSection& PlayTradeXSection =
        Menu->AddSection(
            TEXT("PlayTradeX.Main"),
            LOCTEXT(
                "PlayTradeXSection",
                "PlayTradeX"
            )
        );

    // ========================================================
    // ABI Converter
    // ========================================================

    PlayTradeXSection.AddMenuEntry(
        TEXT("PlayTradeX.AbiConverter"),
        LOCTEXT(
            "AbiConverterLabel",
            "ABI Converter"
        ),
        LOCTEXT(
            "AbiConverterTooltip",
            "Convert a contract ABI and generate a PlayTradeX contract interface"
        ),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(
                this,
                &FPlayTradeXEditorModule::OpenAbiConverter
            )
        )
    );

    // ========================================================
    // Settings
    // ========================================================

    FToolMenuSection& SettingsSection =
        Menu->AddSection(
            TEXT("PlayTradeX.Settings")
        );


    PlayTradeXSection.AddMenuEntry(
        TEXT("PlayTradeX.OpenSettings"),
        LOCTEXT(
            "OpenSettingsLabel",
            "Open Settings"
        ),
        LOCTEXT(
            "OpenSettingsTooltip",
            "Open PlayTradeX Project Settings"
        ),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(
                this,
                &FPlayTradeXEditorModule::OpenSettings
            )
        )
    );

    // ========================================================
    // Import Sample
    // ========================================================

    PlayTradeXSection.AddMenuEntry(
        TEXT("PlayTradeX.ImportSample"),
        LOCTEXT(
            "ImportSampleLabel",
            "Import Sample"
        ),
        LOCTEXT(
            "ImportSampleTooltip",
            "Import the PlayTradeX sample map into the current project"
        ),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(
                this,
                &FPlayTradeXEditorModule::ImportSample
            )
        )
    );

    // ========================================================
    // About
    // ========================================================

    PlayTradeXSection.AddMenuEntry(
        TEXT("PlayTradeX.About"),
        LOCTEXT(
            "AboutLabel",
            "About"
        ),
        LOCTEXT(
            "AboutTooltip",
            "View information about the PlayTradeX Unreal SDK"
        ),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(
                this,
                &FPlayTradeXEditorModule::OpenAbout
            )
        )
    );
}


// ============================================================
// About
// ============================================================

void FPlayTradeXEditorModule::OpenAbout()
{
    FPlayTradeXAboutWindow::Open();
}


// ============================================================
// ABI Converter
// ============================================================

void FPlayTradeXEditorModule::OpenAbiConverter()
{
    FPlayTradeXAbiConverter::Open();
}


// ============================================================
// Import Sample
// ============================================================

void FPlayTradeXEditorModule::ImportSample()
{
    // ========================================================
    // Verify PlayTradeX Plugin
    // ========================================================

    const TSharedPtr<IPlugin> PlayTradeXPlugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );

    if (!PlayTradeXPlugin.IsValid())
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "ImportSamplePluginNotFound",
                "The PlayTradeX plugin could not be located."
            )
        );

        return;
    }


    // ========================================================
    // Source And Destination
    // ========================================================

    const FString SourceRoot =
        TEXT("/PlayTradeX/Samples");

    const FString DestinationRoot =
        TEXT("/Game/PlayTradeX/Samples");


    // ========================================================
    // Load Asset Registry
    // ========================================================

    FAssetRegistryModule& AssetRegistryModule =
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
            TEXT("AssetRegistry")
        );

    IAssetRegistry& AssetRegistry =
        AssetRegistryModule.Get();


    // ========================================================
    // Make Sure Plugin Assets Are Discovered
    // ========================================================

    TArray<FString> SourcePaths;

    SourcePaths.Add(
        SourceRoot
    );

    AssetRegistry.ScanPathsSynchronous(
        SourcePaths,
        true
    );


    // ========================================================
    // Find All Sample Assets
    // ========================================================

    FARFilter Filter;

    Filter.PackagePaths.Add(
        FName(*SourceRoot)
    );

    Filter.bRecursivePaths = true;


    TArray<FAssetData> SourceAssets;

    AssetRegistry.GetAssets(
        Filter,
        SourceAssets
    );


    if (SourceAssets.IsEmpty())
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "ImportSampleNoAssets",
                "No PlayTradeX sample assets were found.\n\n"
                "Expected plugin content path:\n"
                "/PlayTradeX/Samples"
            )
        );

        return;
    }


    // ========================================================
    // Check Existing Project Sample
    // ========================================================

    FARFilter ExistingFilter;

    ExistingFilter.PackagePaths.Add(
        FName(*DestinationRoot)
    );

    ExistingFilter.bRecursivePaths = true;


    TArray<FAssetData> ExistingAssets;

    AssetRegistry.GetAssets(
        ExistingFilter,
        ExistingAssets
    );


    if (!ExistingAssets.IsEmpty())
    {
        const EAppReturnType::Type ReplaceResult =
            FMessageDialog::Open(
                EAppMsgType::YesNo,
                LOCTEXT(
                    "ImportSampleReplace",
                    "PlayTradeX Samples already exist in this project.\n\n"
                    "Do you want to replace them with the samples "
                    "included with this version of PlayTradeX?"
                ),
                LOCTEXT(
                    "ImportSampleReplaceTitle",
                    "Replace PlayTradeX Samples"
                )
            );


        if (ReplaceResult != EAppReturnType::Yes)
        {
            return;
        }


        // ====================================================
        // Delete Existing Imported Assets
        // ====================================================

        TArray<UObject*> ObjectsToDelete;


        for (const FAssetData& AssetData : ExistingAssets)
        {
            UObject* Asset =
                AssetData.GetAsset();

            if (Asset != nullptr)
            {
                ObjectsToDelete.Add(
                    Asset
                );
            }
        }


        if (!ObjectsToDelete.IsEmpty())
        {
            const int32 DeletedCount =
                ObjectTools::DeleteObjects(
                    ObjectsToDelete,
                    false
                );


            if (DeletedCount < ObjectsToDelete.Num())
            {
                FMessageDialog::Open(
                    EAppMsgType::Ok,
                    LOCTEXT(
                        "ImportSampleDeleteFailed",
                        "PlayTradeX could not replace all existing "
                        "sample assets.\n\n"
                        "Close any PlayTradeX sample assets that are "
                        "currently open and try again."
                    )
                );

                return;
            }
        }
    }


    // ========================================================
    // Load Asset Tools
    // ========================================================

    FAssetToolsModule& AssetToolsModule =
        FModuleManager::LoadModuleChecked<FAssetToolsModule>(
            TEXT("AssetTools")
        );

    IAssetTools& AssetTools =
        AssetToolsModule.Get();


    // ========================================================
    // Collect Source Objects
    // ========================================================

    TArray<UObject*> SourceObjects;

    SourceObjects.Reserve(
        SourceAssets.Num()
    );


    for (const FAssetData& AssetData : SourceAssets)
    {
        UObject* Asset =
            AssetData.GetAsset();

        if (Asset != nullptr)
        {
            SourceObjects.Add(
                Asset
            );
        }
    }


    if (SourceObjects.IsEmpty())
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "ImportSampleLoadFailed",
                "PlayTradeX found the sample assets, but Unreal Engine "
                "could not load them."
            )
        );

        return;
    }


    // ========================================================
    // Duplicate Sample Assets
    //
    // Unreal performs the duplication rather than copying
    // package files directly.
    // ========================================================

    TArray<UObject*> ImportedAssets;


    for (UObject* SourceAsset : SourceObjects)
    {
        if (SourceAsset == nullptr)
        {
            continue;
        }


        const FString SourcePackageName =
            SourceAsset->GetOutermost()->GetName();


        FString RelativePackagePath =
            SourcePackageName;


        if (!RelativePackagePath.RemoveFromStart(
            SourceRoot
        ))
        {
            continue;
        }


        FString RelativeDirectory =
            FPackageName::GetLongPackagePath(
                RelativePackagePath
            );


        FString DestinationPackagePath =
            DestinationRoot;


        if (!RelativeDirectory.IsEmpty())
        {
            DestinationPackagePath =
                DestinationRoot +
                RelativeDirectory;
        }


        const FString AssetName =
            SourceAsset->GetName();


        UObject* DuplicatedAsset =
            AssetTools.DuplicateAsset(
                AssetName,
                DestinationPackagePath,
                SourceAsset
            );


        if (DuplicatedAsset == nullptr)
        {
            FMessageDialog::Open(
                EAppMsgType::Ok,
                FText::Format(
                    LOCTEXT(
                        "ImportSampleDuplicateFailed",
                        "PlayTradeX could not import the sample asset:\n\n"
                        "{0}"
                    ),
                    FText::FromString(
                        SourcePackageName
                    )
                )
            );

            return;
        }


        ImportedAssets.Add(
            DuplicatedAsset
        );
    }


    if (ImportedAssets.IsEmpty())
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "ImportSampleNothingImported",
                "No PlayTradeX sample assets were imported."
            )
        );

        return;
    }


    // ========================================================
    // Save Imported Assets
    // ========================================================

    TArray<UPackage*> PackagesToSave;


    for (UObject* ImportedAsset : ImportedAssets)
    {
        if (ImportedAsset == nullptr)
        {
            continue;
        }


        UPackage* Package =
            ImportedAsset->GetOutermost();


        if (Package != nullptr)
        {
            PackagesToSave.AddUnique(
                Package
            );
        }
    }


    if (!PackagesToSave.IsEmpty())
    {
        FEditorFileUtils::PromptForCheckoutAndSave(
            PackagesToSave,
            false,
            false
        );
    }


    // ========================================================
    // Refresh Destination
    // ========================================================

    TArray<FString> DestinationPaths;

    DestinationPaths.Add(
        DestinationRoot
    );


    AssetRegistry.ScanPathsSynchronous(
        DestinationPaths,
        true
    );


    // ========================================================
    // Sync Content Browser
    // ========================================================

    TArray<FAssetData> ImportedAssetData;


    for (UObject* ImportedAsset : ImportedAssets)
    {
        if (ImportedAsset != nullptr)
        {
            ImportedAssetData.Add(
                FAssetData(
                    ImportedAsset
                )
            );
        }
    }


    if (!ImportedAssetData.IsEmpty())
    {
        FContentBrowserModule& ContentBrowserModule =
            FModuleManager::LoadModuleChecked<FContentBrowserModule>(
                TEXT("ContentBrowser")
            );


        ContentBrowserModule
            .Get()
            .SyncBrowserToAssets(
                ImportedAssetData
            );
    }


    // ========================================================
    // Main Sample Map
    // ========================================================

    const FString SampleMapPackage =
        TEXT(
            "/Game/PlayTradeX/Samples/Maps/PlayTradeXSample"
        );


    // ========================================================
    // Ask To Open Sample
    // ========================================================

    const EAppReturnType::Type OpenResult =
        FMessageDialog::Open(
            EAppMsgType::YesNo,
            LOCTEXT(
                "ImportSampleSuccess",
                "PlayTradeX Samples were imported successfully.\n\n"
                "Location:\n"
                "Content/PlayTradeX/Samples\n\n"
                "Would you like to open the PlayTradeX sample map now?"
            ),
            LOCTEXT(
                "ImportSampleSuccessTitle",
                "PlayTradeX Samples Imported"
            )
        );


    if (OpenResult != EAppReturnType::Yes)
    {
        return;
    }


    // ========================================================
    // Verify Main Map
    // ========================================================

    if (!FPackageName::DoesPackageExist(
        SampleMapPackage
    ))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "ImportSampleMapMissing",
                "The PlayTradeX sample content was imported, but the "
                "main PlayTradeXSample map could not be found.\n\n"
                "Expected location:\n"
                "Content/PlayTradeX/Samples/Maps/PlayTradeXSample"
            )
        );

        return;
    }


    // ========================================================
    // Open Main Sample Map
    // ========================================================

    const bool bOpened =
        FEditorFileUtils::LoadMap(
            SampleMapPackage,
            false,
            true
        );


    if (!bOpened)
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "ImportSampleOpenFailed",
                "The PlayTradeX sample was imported successfully, "
                "but Unreal Engine could not open the sample map "
                "automatically.\n\n"
                "You can open it manually from:\n"
                "Content/PlayTradeX/Samples/Maps"
            )
        );
    }
}


// ============================================================
// Open Settings
// ============================================================

void FPlayTradeXEditorModule::OpenSettings()
{
    ISettingsModule* SettingsModule =
        FModuleManager::GetModulePtr<ISettingsModule>(
            TEXT("Settings")
        );


    if (SettingsModule == nullptr)
    {
        SettingsModule =
            &FModuleManager::LoadModuleChecked<ISettingsModule>(
                TEXT("Settings")
            );
    }


    const UPlayTradeXSettings* PlayTradeXSettings =
        GetDefault<UPlayTradeXSettings>();


    if (PlayTradeXSettings == nullptr)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "PlayTradeX: Failed to get PlayTradeX settings."
            )
        );

        return;
    }


    const FName ContainerName =
        PlayTradeXSettings->GetContainerName();

    const FName CategoryName =
        PlayTradeXSettings->GetCategoryName();

    const FName SectionName =
        PlayTradeXSettings->GetSectionName();


    SettingsModule->ShowViewer(
        ContainerName,
        CategoryName,
        SectionName
    );
}


#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(
    FPlayTradeXEditorModule,
    PlayTradeXEditor
)