#include "PlayTradeXAboutWindow.h"

#include "Framework/Application/SlateApplication.h"

#include "Interfaces/IPluginManager.h"

#include "Misc/EngineVersion.h"

#include "HAL/PlatformProcess.h"

#include "Styling/AppStyle.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/SWindow.h"

#include "Widgets/Input/SButton.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"

#include "Widgets/Text/STextBlock.h"


#define LOCTEXT_NAMESPACE "FPlayTradeXAboutWindow"


// ============================================================
// Constants
// ============================================================

namespace PlayTradeXAbout
{
    static const TCHAR* DocumentationUrl =
        TEXT("https://github.com/ektishaf/playtradex-sdk-unreal");

    static const TCHAR* RepositoryUrl =
        TEXT("https://github.com/ektishaf/playtradex-sdk-unreal");

    static const TCHAR* IssuesUrl =
        TEXT("https://github.com/ektishaf/playtradex-sdk-unreal/issues");
}


// ============================================================
// Static State
// ============================================================

TWeakPtr<SWindow> FPlayTradeXAboutWindow::AboutWindow;


// ============================================================
// Shared UI Helpers
// ============================================================

namespace
{
    TSharedRef<SWidget> CreateSectionTitle(
        const FText& Text
    )
    {
        return

            SNew(STextBlock)

            .Text(Text)

            .Font(
                FCoreStyle::GetDefaultFontStyle(
                    TEXT("Bold"),
                    10
                )
            );
    }


    TSharedRef<SWidget> CreateInformationRow(
        const FText& Label,
        const FString& Value
    )
    {
        const FString DisplayValue =
            Value.IsEmpty()
            ? TEXT("Unknown")
            : Value;

        return

            SNew(SHorizontalBox)


            // ====================================================
            // Label
            // ====================================================

            +SHorizontalBox::Slot()

            .FillWidth(0.40f)

            .VAlign(VAlign_Center)

            .Padding(
                0.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(Label)

                    .Font(
                        FAppStyle::GetFontStyle(
                            TEXT("PropertyWindow.BoldFont")
                        )
                    )

            ]


        // ====================================================
        // Value
        // ====================================================

        +SHorizontalBox::Slot()

            .FillWidth(0.60f)

            .VAlign(VAlign_Center)

            .Padding(
                8.0f,
                3.0f,
                0.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(
                        FText::FromString(
                            DisplayValue
                        )
                    )

                    .AutoWrapText(true)

            ];
    }


    TSharedRef<SWidget> CreatePlatformRow(
        const FText& Platform,
        const FText& Architecture,
        bool bSupported
    )
    {
        return

            SNew(SHorizontalBox)


            // ====================================================
            // State
            // ====================================================

            +SHorizontalBox::Slot()

            .AutoWidth()

            .VAlign(VAlign_Center)

            .Padding(
                0.0f,
                3.0f,
                10.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(
                        bSupported
                        ? FText::FromString(TEXT("✓"))
                        : FText::FromString(TEXT("○"))
                    )

            ]


        // ====================================================
        // Platform
        // ====================================================

        +SHorizontalBox::Slot()

            .FillWidth(0.40f)

            .VAlign(VAlign_Center)

            .Padding(
                0.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(Platform)

                    .Font(
                        FAppStyle::GetFontStyle(
                            TEXT("PropertyWindow.BoldFont")
                        )
                    )

            ]


        // ====================================================
        // Architecture
        // ====================================================

        +SHorizontalBox::Slot()

            .FillWidth(0.60f)

            .VAlign(VAlign_Center)

            .Padding(
                8.0f,
                3.0f,
                0.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(Architecture)

            ];
    }


    TSharedRef<SWidget> CreateFeatureRow(
        const FText& Feature
    )
    {
        return

            SNew(SHorizontalBox)


            + SHorizontalBox::Slot()

            .AutoWidth()

            .VAlign(VAlign_Top)

            .Padding(
                4.0f,
                3.0f,
                8.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(
                        FText::FromString(
                            TEXT("•")
                        )
                    )

            ]


        + SHorizontalBox::Slot()

            .FillWidth(1.0f)

            .VAlign(VAlign_Top)

            .Padding(
                0.0f,
                3.0f
            )

            [

                SNew(STextBlock)

                    .Text(Feature)

                    .AutoWrapText(true)

            ];
    }
}


// ============================================================
// Open
// ============================================================

void FPlayTradeXAboutWindow::Open()
{
    // ========================================================
    // Existing Window
    // ========================================================

    if (AboutWindow.IsValid())
    {
        const TSharedPtr<SWindow> ExistingWindow =
            AboutWindow.Pin();

        if (ExistingWindow.IsValid())
        {
            ExistingWindow->BringToFront(true);

            return;
        }
    }


    // ========================================================
    // Create
    // ========================================================

    const TSharedRef<SWindow> Window =
        CreateWindow();

    AboutWindow =
        Window;


    // ========================================================
    // Display
    // ========================================================

    FSlateApplication::Get().AddWindow(
        Window
    );
}


// ============================================================
// Create Window
// ============================================================

TSharedRef<SWindow> FPlayTradeXAboutWindow::CreateWindow()
{
    const FString PluginVersion =
        GetPluginVersion();

    const FString ReleaseChannel =
        GetReleaseChannel(
            PluginVersion
        );

    const FString InstallationSource =
        GetInstallationSource();

    const FString InstalledRevision =
        GetInstalledRevision();

    const FString UnrealVersion =
        GetUnrealEngineVersion();

    const FString Disclaimer =
        GetReleaseDisclaimer(
            ReleaseChannel
        );


    // ========================================================
    // Window
    // ========================================================

    TSharedRef<SWindow> Window =

        SNew(SWindow)

        .Title(
            LOCTEXT(
                "WindowTitle",
                "About PlayTradeX"
            )
        )

        .ClientSize(
            FVector2D(
                520.0f,
                700.0f
            )
        )

        .SizingRule(
            ESizingRule::UserSized
        )

        .SupportsMaximize(false)

        .SupportsMinimize(false);


    // ========================================================
    // Content
    // ========================================================

    Window->SetContent(

        SNew(SBorder)

        .BorderImage(
            FAppStyle::GetBrush(
                TEXT("Brushes.Panel")
            )
        )

        .Padding(
            20.0f
        )

        [

            SNew(SScrollBox)


                // ====================================================
                // Header
                // ====================================================

                +SScrollBox::Slot()

                [

                    SNew(SVerticalBox)


                        + SVerticalBox::Slot()

                        .AutoHeight()

                        .HAlign(HAlign_Center)

                        .Padding(
                            0.0f,
                            8.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(STextBlock)

                                .Text(
                                    LOCTEXT(
                                        "ProductTitle",
                                        "PlayTradeX SDK"
                                    )
                                )

                                .Font(
                                    FCoreStyle::GetDefaultFontStyle(
                                        TEXT("Bold"),
                                        14
                                    )
                                )

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .HAlign(HAlign_Center)

                        .Padding(
                            10.0f,
                            8.0f,
                            10.0f,
                            0.0f
                        )

                        [

                            SNew(STextBlock)

                                .Text(
                                    LOCTEXT(
                                        "ProductSubtitle",
                                        "Cross Platform Blockchain SDK for Games"
                                    )
                                )

                                .Justification(
                                    ETextJustify::Center
                                )

                                .AutoWrapText(true)

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .HAlign(HAlign_Center)

                        .Padding(
                            10.0f,
                            5.0f,
                            10.0f,
                            0.0f
                        )

                        [

                            SNew(STextBlock)

                                .Text(
                                    LOCTEXT(
                                        "IntegrationSubtitle",
                                        "Native EVM integration for Unreal Engine"
                                    )
                                )

                                .Justification(
                                    ETextJustify::Center
                                )

                                .AutoWrapText(true)

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .HAlign(HAlign_Center)

                        .Padding(
                            0.0f,
                            10.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(STextBlock)

                                .Text(
                                    FText::Format(
                                        LOCTEXT(
                                            "VersionText",
                                            "Version {0}"
                                        ),
                                        FText::FromString(
                                            PluginVersion
                                        )
                                    )
                                )

                                .Font(
                                    FAppStyle::GetFontStyle(
                                        TEXT("SmallFont")
                                    )
                                )

                        ]


                    // =================================================
                    // Release Information
                    // =================================================

                    +SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            28.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            CreateSectionTitle(
                                LOCTEXT(
                                    "ReleaseInformationTitle",
                                    "Release Information"
                                )
                            )

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            6.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(SBorder)

                                .BorderImage(
                                    FAppStyle::GetBrush(
                                        TEXT("ToolPanel.GroupBorder")
                                    )
                                )

                                .Padding(
                                    12.0f
                                )

                                [

                                    SNew(SVerticalBox)


                                        + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreateInformationRow(
                                                LOCTEXT(
                                                    "VersionLabel",
                                                    "Version"
                                                ),
                                                PluginVersion
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreateInformationRow(
                                                LOCTEXT(
                                                    "ReleaseChannelLabel",
                                                    "Release Channel"
                                                ),
                                                ReleaseChannel
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreateInformationRow(
                                                LOCTEXT(
                                                    "UnrealLabel",
                                                    "Unreal Engine"
                                                ),
                                                UnrealVersion
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreateInformationRow(
                                                LOCTEXT(
                                                    "InstallationLabel",
                                                    "Installation"
                                                ),
                                                InstallationSource
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreateInformationRow(
                                                LOCTEXT(
                                                    "RevisionLabel",
                                                    "Revision"
                                                ),
                                                InstalledRevision
                                            )

                                        ]

                                ]

                        ]


                    // =================================================
                    // Supported Platforms
                    // =================================================

                    +SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            20.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            CreateSectionTitle(
                                LOCTEXT(
                                    "SupportedPlatformsTitle",
                                    "Supported Platforms"
                                )
                            )

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            6.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(SBorder)

                                .BorderImage(
                                    FAppStyle::GetBrush(
                                        TEXT("ToolPanel.GroupBorder")
                                    )
                                )

                                .Padding(
                                    12.0f
                                )

                                [

                                    SNew(SVerticalBox)


                                        + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreatePlatformRow(
                                                LOCTEXT(
                                                    "WindowsPlatform",
                                                    "Windows"
                                                ),
                                                LOCTEXT(
                                                    "WindowsArchitecture",
                                                    "x64"
                                                ),
                                                true
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreatePlatformRow(
                                                LOCTEXT(
                                                    "AndroidPlatform",
                                                    "Android"
                                                ),
                                                LOCTEXT(
                                                    "AndroidArchitecture",
                                                    "ARM64 (arm64 v8a)"
                                                ),
                                                true
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreatePlatformRow(
                                                LOCTEXT(
                                                    "MacPlatform",
                                                    "macOS"
                                                ),
                                                LOCTEXT(
                                                    "MacStatus",
                                                    "Planned"
                                                ),
                                                false
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreatePlatformRow(
                                                LOCTEXT(
                                                    "LinuxPlatform",
                                                    "Linux"
                                                ),
                                                LOCTEXT(
                                                    "LinuxStatus",
                                                    "Planned"
                                                ),
                                                false
                                            )

                                        ]


                                    + SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            CreatePlatformRow(
                                                LOCTEXT(
                                                    "IosPlatform",
                                                    "iOS"
                                                ),
                                                LOCTEXT(
                                                    "IosStatus",
                                                    "Planned"
                                                ),
                                                false
                                            )

                                        ]

                                ]

                        ]


                    // =================================================
                    // SDK Features
                    // =================================================

                    +SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            20.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            CreateSectionTitle(
                                LOCTEXT(
                                    "SdkFeaturesTitle",
                                    "SDK Features"
                                )
                            )

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            6.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(SBorder)

                                .BorderImage(
                                    FAppStyle::GetBrush(
                                        TEXT("ToolPanel.GroupBorder")
                                    )
                                )

                                .Padding(
                                    10.0f
                                )

                                [

                                    SNew(SVerticalBox)


                                        + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureMultiNetwork",
                                                    "Multi network EVM integration"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureWallets",
                                                    "SDK managed and external wallets"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureNativeTransactions",
                                                    "Native currency balances and transactions"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureContracts",
                                                    "Smart contract read and write operations"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureSimulation",
                                                    "Transaction simulation and fee estimation"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureConsent",
                                                    "Transaction consent and approval flow"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureHumanAbi",
                                                    "Human readable ABI support"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureAbiConverter",
                                                    "ABI Converter and generated configuration IDs"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureWalletImportExport",
                                                    "Encrypted wallet import and export"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureSecureStorage",
                                                    "Secure platform wallet storage"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureUnits",
                                                    "Exact blockchain unit conversion"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureRequests",
                                                    "Sequential and concurrent request execution"
                                                )
                                            )
                                        ]


                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        [
                                            CreateFeatureRow(
                                                LOCTEXT(
                                                    "FeatureAsync",
                                                    "Asynchronous Unreal Engine integration"
                                                )
                                            )
                                        ]

                                ]

                        ]


                    // =================================================
                    // Resources
                    // =================================================

                    +SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            20.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            CreateSectionTitle(
                                LOCTEXT(
                                    "ResourcesTitle",
                                    "Resources"
                                )
                            )

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .Padding(
                            0.0f,
                            6.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(SBorder)

                                .BorderImage(
                                    FAppStyle::GetBrush(
                                        TEXT("ToolPanel.GroupBorder")
                                    )
                                )

                                .Padding(
                                    10.0f
                                )

                                [

                                    SNew(SVerticalBox)


                                        // =========================================
                                        // First Button Row
                                        // =========================================

                                        +SVerticalBox::Slot()

                                        .AutoHeight()

                                        [

                                            SNew(SHorizontalBox)


                                                + SHorizontalBox::Slot()

                                                .FillWidth(1.0f)

                                                .Padding(
                                                    0.0f,
                                                    0.0f,
                                                    3.0f,
                                                    0.0f
                                                )

                                                [

                                                    SNew(SButton)

                                                        .HAlign(HAlign_Center)

                                                        .ContentPadding(
                                                            FMargin(
                                                                10.0f,
                                                                6.0f
                                                            )
                                                        )

                                                        .Text(
                                                            LOCTEXT(
                                                                "DocumentationButton",
                                                                "Documentation"
                                                            )
                                                        )

                                                        .OnClicked_Lambda(
                                                            []()
                                                            {
                                                                FPlayTradeXAboutWindow::
                                                                    OpenDocumentation();

                                                                return
                                                                    FReply::Handled();
                                                            }
                                                        )

                                                ]


                                            + SHorizontalBox::Slot()

                                                .FillWidth(1.0f)

                                                .Padding(
                                                    3.0f,
                                                    0.0f,
                                                    0.0f,
                                                    0.0f
                                                )

                                                [

                                                    SNew(SButton)

                                                        .HAlign(HAlign_Center)

                                                        .ContentPadding(
                                                            FMargin(
                                                                10.0f,
                                                                6.0f
                                                            )
                                                        )

                                                        .Text(
                                                            LOCTEXT(
                                                                "GitHubButton",
                                                                "GitHub"
                                                            )
                                                        )

                                                        .OnClicked_Lambda(
                                                            []()
                                                            {
                                                                FPlayTradeXAboutWindow::
                                                                    OpenRepository();

                                                                return
                                                                    FReply::Handled();
                                                            }
                                                        )

                                                ]

                                        ]


                                    // =========================================
                                    // Second Button Row
                                    // =========================================

                                    +SVerticalBox::Slot()

                                        .AutoHeight()

                                        .Padding(
                                            0.0f,
                                            6.0f,
                                            0.0f,
                                            0.0f
                                        )

                                        [

                                            SNew(SHorizontalBox)


                                                + SHorizontalBox::Slot()

                                                .FillWidth(1.0f)

                                                .Padding(
                                                    0.0f,
                                                    0.0f,
                                                    3.0f,
                                                    0.0f
                                                )

                                                [

                                                    SNew(SButton)

                                                        .HAlign(HAlign_Center)

                                                        .ContentPadding(
                                                            FMargin(
                                                                10.0f,
                                                                6.0f
                                                            )
                                                        )

                                                        .Text(
                                                            LOCTEXT(
                                                                "CheckUpdatesButton",
                                                                "Check for Updates"
                                                            )
                                                        )

                                                        .OnClicked_Lambda(
                                                            []()
                                                            {
                                                                FPlayTradeXAboutWindow::
                                                                    OpenUpdater();

                                                                return
                                                                    FReply::Handled();
                                                            }
                                                        )

                                                ]


                                            + SHorizontalBox::Slot()

                                                .FillWidth(1.0f)

                                                .Padding(
                                                    3.0f,
                                                    0.0f,
                                                    0.0f,
                                                    0.0f
                                                )

                                                [

                                                    SNew(SButton)

                                                        .HAlign(HAlign_Center)

                                                        .ContentPadding(
                                                            FMargin(
                                                                10.0f,
                                                                6.0f
                                                            )
                                                        )

                                                        .Text(
                                                            LOCTEXT(
                                                                "ReportIssueButton",
                                                                "Report Issue"
                                                            )
                                                        )

                                                        .OnClicked_Lambda(
                                                            []()
                                                            {
                                                                FPlayTradeXAboutWindow::
                                                                    OpenIssues();

                                                                return
                                                                    FReply::Handled();
                                                            }
                                                        )

                                                ]

                                        ]

                                ]

                        ]


                    // =================================================
                    // Footer
                    // =================================================

                    +SVerticalBox::Slot()

                        .AutoHeight()

                        .HAlign(HAlign_Center)

                        .Padding(
                            0.0f,
                            26.0f,
                            0.0f,
                            0.0f
                        )

                        [

                            SNew(STextBlock)

                                .Text(
                                    LOCTEXT(
                                        "Copyright",
                                        "PlayTradeX © 2026"
                                    )
                                )

                                .Font(
                                    FAppStyle::GetFontStyle(
                                        TEXT("SmallFont")
                                    )
                                )

                        ]


                    + SVerticalBox::Slot()

                        .AutoHeight()

                        .HAlign(HAlign_Center)

                        .Padding(
                            16.0f,
                            8.0f,
                            16.0f,
                            20.0f
                        )

                        [

                            SNew(STextBlock)

                                .Visibility(
                                    Disclaimer.IsEmpty()
                                    ? EVisibility::Collapsed
                                    : EVisibility::Visible
                                )

                                .Text(
                                    FText::FromString(
                                        Disclaimer
                                    )
                                )

                                .Font(
                                    FAppStyle::GetFontStyle(
                                        TEXT("SmallFont")
                                    )
                                )

                                .Justification(
                                    ETextJustify::Center
                                )

                                .AutoWrapText(true)

                        ]

                ]

        ]

    );


    return Window;
}


// ============================================================
// Plugin Version
// ============================================================

FString FPlayTradeXAboutWindow::GetPluginVersion()
{
    const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );

    if (!Plugin.IsValid())
    {
        return TEXT("Unknown");
    }

    const FString VersionName =
        Plugin->GetDescriptor().VersionName;

    if (!VersionName.IsEmpty())
    {
        return VersionName;
    }

    return FString::FromInt(
        Plugin->GetDescriptor().Version
    );
}


// ============================================================
// Release Channel
// ============================================================

FString FPlayTradeXAboutWindow::GetReleaseChannel(
    const FString& Version
)
{
    const FString LowerVersion =
        Version.ToLower();

    if (LowerVersion.Contains(
        TEXT("alpha")
    ))
    {
        return TEXT("Alpha");
    }

    if (LowerVersion.Contains(
        TEXT("beta")
    ))
    {
        return TEXT("Beta");
    }

    if (
        LowerVersion.Contains(
            TEXT("rc")
        ) ||
        LowerVersion.Contains(
            TEXT("releasecandidate")
        ) ||
        LowerVersion.Contains(
            TEXT("release candidate")
        )
        )
    {
        return TEXT("Release Candidate");
    }

    if (
        !Version.IsEmpty() &&
        !Version.Equals(
            TEXT("Unknown"),
            ESearchCase::IgnoreCase
        )
        )
    {
        return TEXT("Stable");
    }

    return TEXT("Unknown");
}


// ============================================================
// Installation Source
// ============================================================

FString FPlayTradeXAboutWindow::GetInstallationSource()
{
    const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );

    if (!Plugin.IsValid())
    {
        return TEXT("Unknown");
    }

    const FString BaseDirectory =
        Plugin->GetBaseDir();

    if (BaseDirectory.IsEmpty())
    {
        return TEXT("Unknown");
    }

    if (
        BaseDirectory.Contains(
            TEXT("/Plugins/")
        ) ||
        BaseDirectory.Contains(
            TEXT("\\Plugins\\")
        )
        )
    {
        return TEXT("Project Plugin");
    }

    return TEXT("Engine Plugin");
}


// ============================================================
// Installed Revision
// ============================================================

FString FPlayTradeXAboutWindow::GetInstalledRevision()
{
    /*
     * The current Unreal plugin does not yet maintain an
     * installed Git revision metadata value equivalent to the
     * Unity package updater.
     *
     * The SDK Updater will become the authoritative source for
     * this value once its Unreal implementation is added.
     */

    return TEXT("Local Development");
}


// ============================================================
// Unreal Engine Version
// ============================================================

FString FPlayTradeXAboutWindow::GetUnrealEngineVersion()
{
    return FEngineVersion::Current().ToString();
}


// ============================================================
// Release Disclaimer
// ============================================================

FString FPlayTradeXAboutWindow::GetReleaseDisclaimer(
    const FString& ReleaseChannel
)
{
    if (ReleaseChannel.Equals(
        TEXT("Alpha"),
        ESearchCase::IgnoreCase
    ))
    {
        return
            TEXT(
                "Alpha software intended for development, "
                "integration and testing."
            );
    }

    if (ReleaseChannel.Equals(
        TEXT("Beta"),
        ESearchCase::IgnoreCase
    ))
    {
        return
            TEXT(
                "Beta software intended for development, "
                "integration and pre release testing."
            );
    }

    if (ReleaseChannel.Equals(
        TEXT("Release Candidate"),
        ESearchCase::IgnoreCase
    ))
    {
        return
            TEXT(
                "Release candidate intended for final integration "
                "and validation before stable release."
            );
    }

    if (ReleaseChannel.Equals(
        TEXT("Stable"),
        ESearchCase::IgnoreCase
    ))
    {
        return
            TEXT(
                "Stable PlayTradeX SDK release."
            );
    }

    return FString();
}


// ============================================================
// URL
// ============================================================

void FPlayTradeXAboutWindow::OpenUrl(
    const FString& Url
)
{
    if (Url.IsEmpty())
    {
        return;
    }

    FPlatformProcess::LaunchURL(
        *Url,
        nullptr,
        nullptr
    );
}


// ============================================================
// Documentation
// ============================================================

void FPlayTradeXAboutWindow::OpenDocumentation()
{
    OpenUrl(
        PlayTradeXAbout::DocumentationUrl
    );
}


// ============================================================
// Repository
// ============================================================

void FPlayTradeXAboutWindow::OpenRepository()
{
    OpenUrl(
        PlayTradeXAbout::RepositoryUrl
    );
}


// ============================================================
// Issues
// ============================================================

void FPlayTradeXAboutWindow::OpenIssues()
{
    OpenUrl(
        PlayTradeXAbout::IssuesUrl
    );
}


// ============================================================
// SDK Updater
// ============================================================

void FPlayTradeXAboutWindow::OpenUpdater()
{
    /*
     * The Unreal SDK Updater window will replace this
     * temporary action when we implement it.
     *
     * Keeping the button now preserves parity with the
     * Unity About window.
     */

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: SDK Updater requested from About window."
        )
    );
}


#undef LOCTEXT_NAMESPACE