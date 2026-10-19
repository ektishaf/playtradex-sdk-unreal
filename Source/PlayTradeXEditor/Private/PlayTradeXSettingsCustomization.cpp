#include "PlayTradeXSettingsCustomization.h"
#include "PlayTradeX.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PlayTradeXSettings.h"

#include "Async/Async.h"
#include "Dom/JsonObject.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "HAL/FileManager.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "PlayTradeXSettingsCustomization"

TSharedRef<IDetailCustomization>
FPlayTradeXSettingsCustomization::MakeInstance()
{
    return MakeShared<FPlayTradeXSettingsCustomization>();
}

void FPlayTradeXSettingsCustomization::CustomizeDetails(
    IDetailLayoutBuilder& InDetailBuilder)
{
    DetailBuilder = &InDetailBuilder;

    TArray<TWeakObjectPtr<UObject>> CustomizedObjects;

    InDetailBuilder.GetObjectsBeingCustomized(
        CustomizedObjects
    );

    if (CustomizedObjects.Num() > 0)
    {
        Settings = Cast<UPlayTradeXSettings>(
            CustomizedObjects[0].Get()
        );
    }

    IDetailCategoryBuilder& NetworksCategory =
        InDetailBuilder.EditCategory(
            TEXT("Networks"),
            LOCTEXT(
                "NetworksCategory",
                "Networks"
            )
        );

    NetworksCategory.AddCustomRow(
        LOCTEXT(
            "PlayTradeXNetworkActionsSearch",
            "PlayTradeX Network Actions"
        )
    )
        .WholeRowContent()
        [
            SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SNew(STextBlock)
                        .Text(
                            LOCTEXT(
                                "NetworksLabel",
                                "Networks"
                            )
                        )
                ]

            + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.0f, 0.0f, 4.0f, 0.0f)
                [
                    SNew(SBox)
                        .WidthOverride(125.0f)
                        [
                            SNew(SComboButton)
                                .ButtonContent()
                                [
                                    SNew(STextBlock)
                                        .Text(
                                            LOCTEXT(
                                                "AddTestnetButton",
                                                "Add Testnet ▼"
                                            )
                                        )
                                ]
                            .OnGetMenuContent(
                                this,
                                &FPlayTradeXSettingsCustomization::
                                BuildTestnetMenu
                            )
                        ]
                ]

            + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.0f, 0.0f, 4.0f, 0.0f)
                [
                    SNew(SBox)
                        .WidthOverride(125.0f)
                        [
                            SNew(SButton)
                                .HAlign(HAlign_Center)
                                .Text(
                                    LOCTEXT(
                                        "AddAllTestnetsButton",
                                        "Add All Testnets"
                                    )
                                )
                                .OnClicked(
                                    this,
                                    &FPlayTradeXSettingsCustomization::
                                    OnAddAllTestnetsClicked
                                )
                        ]
                ]

            + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.0f, 0.0f, 4.0f, 0.0f)
                [
                    SNew(SBox)
                        .WidthOverride(125.0f)
                        [
                            SNew(SButton)
                                .HAlign(HAlign_Center)
                                .Text(
                                    LOCTEXT(
                                        "RemoveTestnetsButton",
                                        "Remove Testnets"
                                    )
                                )
                                .OnClicked(
                                    this,
                                    &FPlayTradeXSettingsCustomization::
                                    OnRemoveTestnetsClicked
                                )
                        ]
                ]

            + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SBox)
                        .WidthOverride(130.0f)
                        [
                            SNew(SButton)
                                .HAlign(HAlign_Center)
                                .Text(
                                    LOCTEXT(
                                        "GenerateNetworksClassButton",
                                        "Generate Class"
                                    )
                                )
                                .OnClicked(
                                    this,
                                    &FPlayTradeXSettingsCustomization::
                                    OnGenerateNetworksClassClicked
                                )
                        ]
                ]
        ];

    NetworksCategory.AddCustomRow(
        LOCTEXT(
            "PlayTradeXNetworksHelpSearch",
            "Configure blockchain networks PlayTradeX Generate Class"
        )
    )
        .WholeRowContent()
        [
            SNew(STextBlock)
                .AutoWrapText(true)
                .Text(
                    LOCTEXT(
                        "NetworksHelp",
                        "Configure the blockchain networks available to PlayTradeX. "
                        "Generate Class creates GeneratedNetworks.h containing "
                        "constants for the configured network IDs."
                    )
                )
        ];

    IDetailCategoryBuilder& WalletsCategory =
        InDetailBuilder.EditCategory(
            TEXT("Wallets"),
            LOCTEXT(
                "WalletsCategory",
                "Wallets"
            )
        );

    WalletsCategory.AddCustomRow(
        LOCTEXT(
            "PlayTradeXWalletActionsSearch",
            "PlayTradeX Wallet Actions"
        )
    )
        .WholeRowContent()
        [
            SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SNew(STextBlock)
                        .Text(
                            LOCTEXT(
                                "WalletActionsLabel",
                                "Wallets"
                            )
                        )
                ]

            + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.0f, 0.0f, 4.0f, 0.0f)
                [
                    SNew(SBox)
                        .WidthOverride(130.0f)
                        [
                            SNew(SButton)
                                .HAlign(HAlign_Center)
                                .Text(
                                    LOCTEXT(
                                        "GenerateWalletClassButton",
                                        "Generate Class"
                                    )
                                )
                                .OnClicked(
                                    this,
                                    &FPlayTradeXSettingsCustomization::
                                    OnGenerateWalletsClassClicked
                                )
                        ]
                ]

            + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SBox)
                        .WidthOverride(130.0f)
                        [
                            SNew(SButton)
                                .HAlign(HAlign_Center)
                                .Text(
                                    LOCTEXT(
                                        "GenerateWalletButton",
                                        "Generate Wallet"
                                    )
                                )
                                .OnClicked(
                                    this,
                                    &FPlayTradeXSettingsCustomization::
                                    OnGenerateWalletClicked
                                )
                        ]
                ]
        ];

    WalletsCategory.AddCustomRow(
        LOCTEXT(
            "PlayTradeXWalletHelpSearch",
            "Wallets Generate Wallet Generate Class"
        )
    )
        .WholeRowContent()
        [
            SNew(STextBlock)
                .AutoWrapText(true)
                .Text(
                    LOCTEXT(
                        "WalletsHelp",
                        "Wallets may be entered manually or generated by PlayTradeX. "
                        "Generate Class creates GeneratedWallets.h containing only "
                        "the configured wallet IDs."
                    )
                )
        ];
}

TSharedRef<SWidget>
FPlayTradeXSettingsCustomization::BuildTestnetMenu()
{
    FMenuBuilder MenuBuilder(
        true,
        nullptr
    );

    TArray<FPlayTradeXTestnetPreset> Presets;

    if (!LoadTestnetCatalog(Presets))
    {
        MenuBuilder.AddMenuEntry(
            LOCTEXT(
                "TestnetCatalogUnavailable",
                "Testnet catalog unavailable"
            ),
            LOCTEXT(
                "TestnetCatalogUnavailableTooltip",
                "PlayTradeX could not load evm-testnets.json."
            ),
            FSlateIcon(),
            FUIAction()
        );

        return MenuBuilder.MakeWidget();
    }

    TMap<FString, TArray<FPlayTradeXTestnetPreset>>
        PresetsByEcosystem;

    for (const FPlayTradeXTestnetPreset& Preset : Presets)
    {
        PresetsByEcosystem
            .FindOrAdd(Preset.Ecosystem)
            .Add(Preset);
    }

    TArray<FString> Ecosystems;

    PresetsByEcosystem.GetKeys(Ecosystems);

    Ecosystems.Sort();

    for (const FString& Ecosystem : Ecosystems)
    {
        const TArray<FPlayTradeXTestnetPreset>*
            EcosystemPresets =
            PresetsByEcosystem.Find(Ecosystem);

        if (EcosystemPresets == nullptr)
        {
            continue;
        }

        const TArray<FPlayTradeXTestnetPreset>
            PresetCopies = *EcosystemPresets;

        MenuBuilder.AddSubMenu(
            FText::FromString(Ecosystem),
            FText::Format(
                LOCTEXT(
                    "EcosystemTooltip",
                    "{0} test networks"
                ),
                FText::FromString(Ecosystem)
            ),
            FNewMenuDelegate::CreateLambda(
                [this, PresetCopies](
                    FMenuBuilder& SubMenuBuilder)
                {
                    for (
                        const FPlayTradeXTestnetPreset& Preset :
                        PresetCopies)
                    {
                        const bool bAlreadyConfigured =
                            IsPresetConfigured(Preset);

                        FUIAction Action(
                            FExecuteAction::CreateLambda(
                                [this, Preset]()
                                {
                                    AddTestnet(Preset);
                                }
                            ),
                            FCanExecuteAction::CreateLambda(
                                [bAlreadyConfigured]()
                                {
                                    return !bAlreadyConfigured;
                                }
                            ),
                            FIsActionChecked::CreateLambda(
                                [bAlreadyConfigured]()
                                {
                                    return bAlreadyConfigured;
                                }
                            )
                        );

                        SubMenuBuilder.AddMenuEntry(
                            FText::FromString(
                                Preset.NetworkName
                            ),
                            FText::Format(
                                LOCTEXT(
                                    "AddTestnetTooltip",
                                    "Add {0} to PlayTradeX networks."
                                ),
                                FText::FromString(
                                    Preset.NetworkName
                                )
                            ),
                            FSlateIcon(),
                            Action,
                            NAME_None,
                            EUserInterfaceActionType::ToggleButton
                        );
                    }
                }
            )
        );
    }

    return MenuBuilder.MakeWidget();
}

bool
FPlayTradeXSettingsCustomization::LoadTestnetCatalog(
    TArray<FPlayTradeXTestnetPreset>& OutPresets) const
{
    OutPresets.Reset();

    const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );

    if (!Plugin.IsValid())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "PlayTradeX: Could not locate the PlayTradeX plugin."
            )
        );

        return false;
    }

    const FString CatalogPath =
        FPaths::Combine(
            Plugin->GetBaseDir(),
            TEXT("Resources"),
            TEXT("Networks"),
            TEXT("evm-testnets.json")
        );

    FString JsonText;

    if (!FFileHelper::LoadFileToString(
        JsonText,
        *CatalogPath))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "PlayTradeX: Could not load testnet catalog: %s"
            ),
            *CatalogPath
        );

        return false;
    }

    TSharedPtr<FJsonObject> RootObject;

    const TSharedRef<TJsonReader<>> Reader =
        TJsonReaderFactory<>::Create(
            JsonText
        );

    if (!FJsonSerializer::Deserialize(
        Reader,
        RootObject) ||
        !RootObject.IsValid())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "PlayTradeX: Invalid JSON in testnet catalog: %s"
            ),
            *CatalogPath
        );

        return false;
    }

    const TArray<TSharedPtr<FJsonValue>>*
        NetworkValues = nullptr;

    if (!RootObject->TryGetArrayField(
        TEXT("networks"),
        NetworkValues) ||
        NetworkValues == nullptr)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "PlayTradeX: Testnet catalog has no networks array."
            )
        );

        return false;
    }

    for (
        const TSharedPtr<FJsonValue>& NetworkValue :
        *NetworkValues)
    {
        if (!NetworkValue.IsValid())
        {
            continue;
        }

        const TSharedPtr<FJsonObject> NetworkObject =
            NetworkValue->AsObject();

        if (!NetworkObject.IsValid())
        {
            continue;
        }

        FPlayTradeXTestnetPreset Preset;

        if (!NetworkObject->TryGetStringField(
            TEXT("id"),
            Preset.Id) ||
            Preset.Id.IsEmpty())
        {
            continue;
        }

        NetworkObject->TryGetStringField(
            TEXT("networkName"),
            Preset.NetworkName
        );

        NetworkObject->TryGetStringField(
            TEXT("ecosystem"),
            Preset.Ecosystem
        );

        NetworkObject->TryGetStringField(
            TEXT("symbol"),
            Preset.Symbol
        );

        NetworkObject->TryGetStringField(
            TEXT("blockExplorerUrl"),
            Preset.BlockExplorerUrl
        );

        double ChainIdNumber = 0.0;

        if (NetworkObject->TryGetNumberField(
            TEXT("chainId"),
            ChainIdNumber))
        {
            Preset.ChainId =
                static_cast<int64>(
                    ChainIdNumber
                    );
        }

        bool bIsTestnet = true;

        if (NetworkObject->TryGetBoolField(
            TEXT("isTestnet"),
            bIsTestnet))
        {
            Preset.bIsTestnet =
                bIsTestnet;
        }

        const TArray<TSharedPtr<FJsonValue>>*
            RpcValues = nullptr;

        if (NetworkObject->TryGetArrayField(
            TEXT("rpcUrls"),
            RpcValues) &&
            RpcValues != nullptr)
        {
            for (
                const TSharedPtr<FJsonValue>& RpcValue :
                *RpcValues)
            {
                if (!RpcValue.IsValid())
                {
                    continue;
                }

                FString RpcUrl;

                if (RpcValue->TryGetString(
                    RpcUrl) &&
                    !RpcUrl.IsEmpty())
                {
                    Preset.RpcUrls.Add(
                        MoveTemp(RpcUrl)
                    );
                }
            }
        }

        if (Preset.NetworkName.IsEmpty())
        {
            Preset.NetworkName =
                Preset.Id;
        }

        if (Preset.Ecosystem.IsEmpty())
        {
            Preset.Ecosystem =
                TEXT("Other");
        }

        OutPresets.Add(
            MoveTemp(Preset)
        );
    }

    return OutPresets.Num() > 0;
}

bool
FPlayTradeXSettingsCustomization::IsPresetConfigured(
    const FPlayTradeXTestnetPreset& Preset) const
{
    if (!Settings.IsValid())
    {
        return false;
    }

    for (
        const FPlayTradeXNetworkSettings& Network :
        Settings->Networks)
    {
        if (Network.Id.Equals(
            Preset.Id,
            ESearchCase::IgnoreCase))
        {
            return true;
        }

        if (Network.ChainId > 0 &&
            Network.ChainId == Preset.ChainId)
        {
            return true;
        }
    }

    return false;
}

void
FPlayTradeXSettingsCustomization::AddTestnet(
    FPlayTradeXTestnetPreset Preset)
{
    if (!Settings.IsValid())
    {
        return;
    }

    if (IsPresetConfigured(Preset))
    {
        return;
    }

    UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    SettingsObject->Modify();

    FPlayTradeXNetworkSettings Network;

    Network.Id =
        MoveTemp(Preset.Id);

    Network.NetworkName =
        MoveTemp(Preset.NetworkName);

    Network.ChainId =
        Preset.ChainId;

    Network.Symbol =
        MoveTemp(Preset.Symbol);

    Network.RpcUrls =
        MoveTemp(Preset.RpcUrls);

    Network.BlockExplorerUrl =
        MoveTemp(Preset.BlockExplorerUrl);

    Network.bIsTestnet =
        Preset.bIsTestnet;

    Network.PresetId =
        Network.Id;

    SettingsObject->Networks.Add(
        MoveTemp(Network)
    );

    SaveSettings();

    if (DetailBuilder != nullptr)
    {
        DetailBuilder->ForceRefreshDetails();
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: Added testnet '%s'."
        ),
        *SettingsObject->Networks.Last().Id
    );
}

FReply
FPlayTradeXSettingsCustomization::
OnAddAllTestnetsClicked()
{
    if (!Settings.IsValid())
    {
        return FReply::Handled();
    }

    TArray<FPlayTradeXTestnetPreset> Presets;

    if (!LoadTestnetCatalog(Presets))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "AddAllLoadFailed",
                "PlayTradeX could not load evm-testnets.json."
            )
        );

        return FReply::Handled();
    }

    UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    SettingsObject->Modify();

    int32 AddedCount = 0;
    int32 SkippedCount = 0;

    for (const FPlayTradeXTestnetPreset& Preset : Presets)
    {
        if (IsPresetConfigured(Preset))
        {
            ++SkippedCount;
            continue;
        }

        FPlayTradeXNetworkSettings Network;

        Network.Id =
            Preset.Id;

        Network.NetworkName =
            Preset.NetworkName;

        Network.ChainId =
            Preset.ChainId;

        Network.Symbol =
            Preset.Symbol;

        Network.RpcUrls =
            Preset.RpcUrls;

        Network.BlockExplorerUrl =
            Preset.BlockExplorerUrl;

        Network.bIsTestnet =
            Preset.bIsTestnet;

        Network.PresetId =
            Preset.Id;

        SettingsObject->Networks.Add(
            MoveTemp(Network)
        );

        ++AddedCount;
    }

    if (AddedCount > 0)
    {
        SaveSettings();

        if (DetailBuilder != nullptr)
        {
            DetailBuilder->ForceRefreshDetails();
        }
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: Add All Testnets completed. Added: %d, skipped: %d."
        ),
        AddedCount,
        SkippedCount
    );

    return FReply::Handled();
}

FReply
FPlayTradeXSettingsCustomization::
OnRemoveTestnetsClicked()
{
    if (!Settings.IsValid())
    {
        return FReply::Handled();
    }

    TArray<FPlayTradeXTestnetPreset> Presets;

    if (!LoadTestnetCatalog(Presets))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "RemoveLoadFailed",
                "PlayTradeX could not load evm-testnets.json."
            )
        );

        return FReply::Handled();
    }

    UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    TSet<FString> CatalogIds;
    TSet<int64> CatalogChainIds;

    for (const FPlayTradeXTestnetPreset& Preset : Presets)
    {
        CatalogIds.Add(
            Preset.Id.ToLower()
        );

        CatalogChainIds.Add(
            Preset.ChainId
        );
    }

    int32 RemoveCount = 0;

    for (
        const FPlayTradeXNetworkSettings& Network :
        SettingsObject->Networks)
    {
        const bool bCatalogId =
            CatalogIds.Contains(
                Network.Id.ToLower()
            );

        const bool bCatalogPreset =
            !Network.PresetId.IsEmpty() &&
            CatalogIds.Contains(
                Network.PresetId.ToLower()
            );

        const bool bCatalogChain =
            CatalogChainIds.Contains(
                Network.ChainId
            );

        if (
            Network.bIsTestnet ||
            bCatalogId ||
            bCatalogPreset ||
            bCatalogChain)
        {
            ++RemoveCount;
        }
    }

    if (RemoveCount == 0)
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            LOCTEXT(
                "NoTestnetsToRemove",
                "There are no PlayTradeX testnets to remove."
            )
        );

        return FReply::Handled();
    }

    const EAppReturnType::Type Result =
        FMessageDialog::Open(
            EAppMsgType::YesNo,
            FText::Format(
                LOCTEXT(
                    "ConfirmRemoveTestnets",
                    "Remove {0} configured testnet(s)? "
                    "Custom mainnet configurations will be kept."
                ),
                FText::AsNumber(
                    RemoveCount
                )
            )
        );

    if (Result != EAppReturnType::Yes)
    {
        return FReply::Handled();
    }

    SettingsObject->Modify();

    int32 RemovedCount = 0;

    for (
        int32 Index =
        SettingsObject->Networks.Num() - 1;
        Index >= 0;
        --Index)
    {
        const FPlayTradeXNetworkSettings& Network =
            SettingsObject->Networks[Index];

        const bool bCatalogId =
            CatalogIds.Contains(
                Network.Id.ToLower()
            );

        const bool bCatalogPreset =
            !Network.PresetId.IsEmpty() &&
            CatalogIds.Contains(
                Network.PresetId.ToLower()
            );

        const bool bCatalogChain =
            CatalogChainIds.Contains(
                Network.ChainId
            );

        if (
            Network.bIsTestnet ||
            bCatalogId ||
            bCatalogPreset ||
            bCatalogChain)
        {
            SettingsObject->Networks.RemoveAt(
                Index
            );

            ++RemovedCount;
        }
    }

    if (RemovedCount > 0)
    {
        SaveSettings();

        if (DetailBuilder != nullptr)
        {
            DetailBuilder->ForceRefreshDetails();
        }
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: Removed %d testnet(s)."
        ),
        RemovedCount
    );

    return FReply::Handled();
}

FReply
FPlayTradeXSettingsCustomization::
OnGenerateNetworksClassClicked()
{
    FString ValidationError;

    if (!ValidateNetworks(ValidationError))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::Format(
                LOCTEXT(
                    "GenerateNetworksValidationFailed",
                    "Cannot generate the network class.\n\n{0}"
                ),
                FText::FromString(
                    ValidationError
                )
            )
        );

        return FReply::Handled();
    }

    FString GeneratedPath;
    FString GenerationError;

    if (!GenerateNetworksClass(
        GeneratedPath,
        GenerationError))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::Format(
                LOCTEXT(
                    "GenerateNetworksFailed",
                    "Failed to generate the network class.\n\n{0}"
                ),
                FText::FromString(
                    GenerationError
                )
            )
        );

        return FReply::Handled();
    }

    FMessageDialog::Open(
        EAppMsgType::Ok,
        FText::Format(
            LOCTEXT(
                "GenerateNetworksSucceeded",
                "Generated network class successfully.\n\n{0}"
            ),
            FText::FromString(
                GeneratedPath
            )
        )
    );

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: Generated network class: %s"
        ),
        *GeneratedPath
    );

    return FReply::Handled();
}

bool
FPlayTradeXSettingsCustomization::ValidateNetworks(
    FString& OutError) const
{
    OutError.Empty();

    if (!Settings.IsValid())
    {
        OutError =
            TEXT("PlayTradeX settings are unavailable.");

        return false;
    }

    const UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    if (SettingsObject->Networks.IsEmpty())
    {
        OutError =
            TEXT(
                "Configure at least one network before generating the class."
            );

        return false;
    }

    TSet<FString> NetworkIds;
    TSet<FString> GeneratedIdentifiers;

    for (
        int32 Index = 0;
        Index < SettingsObject->Networks.Num();
        ++Index)
    {
        const FPlayTradeXNetworkSettings& Network =
            SettingsObject->Networks[Index];

        const FString TrimmedId =
            Network.Id.TrimStartAndEnd();

        if (TrimmedId.IsEmpty())
        {
            OutError = FString::Printf(
                TEXT(
                    "Network %d has an empty ID."
                ),
                Index + 1
            );

            return false;
        }

        const FString NormalizedId =
            TrimmedId.ToLower();

        if (NetworkIds.Contains(NormalizedId))
        {
            OutError = FString::Printf(
                TEXT(
                    "Duplicate network ID: %s"
                ),
                *TrimmedId
            );

            return false;
        }

        NetworkIds.Add(NormalizedId);

        if (Network.ChainId <= 0)
        {
            OutError = FString::Printf(
                TEXT(
                    "Network '%s' must have a Chain ID greater than zero."
                ),
                *TrimmedId
            );

            return false;
        }

        if (Network.RpcUrls.IsEmpty())
        {
            OutError = FString::Printf(
                TEXT(
                    "Network '%s' must contain at least one RPC URL."
                ),
                *TrimmedId
            );

            return false;
        }

        for (
            int32 RpcIndex = 0;
            RpcIndex < Network.RpcUrls.Num();
            ++RpcIndex)
        {
            if (
                Network.RpcUrls[RpcIndex]
                .TrimStartAndEnd()
                .IsEmpty())
            {
                OutError = FString::Printf(
                    TEXT(
                        "Network '%s' contains an empty RPC URL at index %d."
                    ),
                    *TrimmedId,
                    RpcIndex
                );

                return false;
            }
        }

        const FString Identifier =
            ToCppIdentifier(TrimmedId);

        if (Identifier.IsEmpty())
        {
            OutError = FString::Printf(
                TEXT(
                    "Network ID '%s' cannot be converted to a valid C++ identifier."
                ),
                *TrimmedId
            );

            return false;
        }

        if (GeneratedIdentifiers.Contains(Identifier))
        {
            OutError = FString::Printf(
                TEXT(
                    "Network ID '%s' generates duplicate C++ identifier '%s'."
                ),
                *TrimmedId,
                *Identifier
            );

            return false;
        }

        GeneratedIdentifiers.Add(
            Identifier
        );
    }

    return true;
}

void FPlayTradeXSettingsCustomization::SaveSettings()
{
    if (!Settings.IsValid())
    {
        return;
    }


    UPlayTradeXSettings* SettingsObject =
        Settings.Get();


    SettingsObject->SaveConfig();

    SettingsObject->UpdateDefaultConfigFile();
}

FString
FPlayTradeXSettingsCustomization::ToCppIdentifier(
    const FString& Value) const
{
    FString Result;

    bool bUppercaseNext = true;

    for (const TCHAR Character : Value)
    {
        if (FChar::IsAlnum(Character))
        {
            if (Result.IsEmpty() &&
                FChar::IsDigit(Character))
            {
                Result.AppendChar(
                    TEXT('_')
                );
            }

            if (bUppercaseNext)
            {
                Result.AppendChar(
                    FChar::ToUpper(Character)
                );

                bUppercaseNext = false;
            }
            else
            {
                Result.AppendChar(Character);
            }
        }
        else
        {
            bUppercaseNext = true;
        }
    }

    if (Result.IsEmpty())
    {
        return Result;
    }

    if (IsCppKeyword(Result))
    {
        Result =
            TEXT("_") + Result;
    }

    return Result;
}

bool
FPlayTradeXSettingsCustomization::IsCppKeyword(
    const FString& Value) const
{
    static const TSet<FString> Keywords =
    {
        TEXT("alignas"),
        TEXT("alignof"),
        TEXT("and"),
        TEXT("and_eq"),
        TEXT("asm"),
        TEXT("auto"),
        TEXT("bitand"),
        TEXT("bitor"),
        TEXT("bool"),
        TEXT("break"),
        TEXT("case"),
        TEXT("catch"),
        TEXT("char"),
        TEXT("char8_t"),
        TEXT("char16_t"),
        TEXT("char32_t"),
        TEXT("class"),
        TEXT("compl"),
        TEXT("concept"),
        TEXT("const"),
        TEXT("consteval"),
        TEXT("constexpr"),
        TEXT("constinit"),
        TEXT("const_cast"),
        TEXT("continue"),
        TEXT("co_await"),
        TEXT("co_return"),
        TEXT("co_yield"),
        TEXT("decltype"),
        TEXT("default"),
        TEXT("delete"),
        TEXT("do"),
        TEXT("double"),
        TEXT("dynamic_cast"),
        TEXT("else"),
        TEXT("enum"),
        TEXT("explicit"),
        TEXT("export"),
        TEXT("extern"),
        TEXT("false"),
        TEXT("float"),
        TEXT("for"),
        TEXT("friend"),
        TEXT("goto"),
        TEXT("if"),
        TEXT("inline"),
        TEXT("int"),
        TEXT("long"),
        TEXT("mutable"),
        TEXT("namespace"),
        TEXT("new"),
        TEXT("noexcept"),
        TEXT("not"),
        TEXT("not_eq"),
        TEXT("nullptr"),
        TEXT("operator"),
        TEXT("or"),
        TEXT("or_eq"),
        TEXT("private"),
        TEXT("protected"),
        TEXT("public"),
        TEXT("register"),
        TEXT("reinterpret_cast"),
        TEXT("requires"),
        TEXT("return"),
        TEXT("short"),
        TEXT("signed"),
        TEXT("sizeof"),
        TEXT("static"),
        TEXT("static_assert"),
        TEXT("static_cast"),
        TEXT("struct"),
        TEXT("switch"),
        TEXT("template"),
        TEXT("this"),
        TEXT("thread_local"),
        TEXT("throw"),
        TEXT("true"),
        TEXT("try"),
        TEXT("typedef"),
        TEXT("typeid"),
        TEXT("typename"),
        TEXT("union"),
        TEXT("unsigned"),
        TEXT("using"),
        TEXT("virtual"),
        TEXT("void"),
        TEXT("volatile"),
        TEXT("wchar_t"),
        TEXT("while"),
        TEXT("xor"),
        TEXT("xor_eq")
    };

    return Keywords.Contains(
        Value.ToLower()
    );
}

FString
FPlayTradeXSettingsCustomization::EscapeCppString(
    const FString& Value) const
{
    FString Result = Value;

    Result.ReplaceInline(
        TEXT("\\"),
        TEXT("\\\\")
    );

    Result.ReplaceInline(
        TEXT("\""),
        TEXT("\\\"")
    );

    Result.ReplaceInline(
        TEXT("\r"),
        TEXT("\\r")
    );

    Result.ReplaceInline(
        TEXT("\n"),
        TEXT("\\n")
    );

    Result.ReplaceInline(
        TEXT("\t"),
        TEXT("\\t")
    );

    return Result;
}

bool
FPlayTradeXSettingsCustomization::GenerateNetworksClass(
    FString& OutGeneratedPath,
    FString& OutError) const
{
    OutGeneratedPath.Empty();
    OutError.Empty();

    if (!Settings.IsValid())
    {
        OutError =
            TEXT("PlayTradeX settings are unavailable.");

        return false;
    }

    const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );

    if (!Plugin.IsValid())
    {
        OutError =
            TEXT(
                "Could not locate the PlayTradeX plugin."
            );

        return false;
    }

    const FString GeneratedDirectory =
        FPaths::Combine(
            Plugin->GetBaseDir(),
            TEXT("Source"),
            TEXT("PlayTradeX"),
            TEXT("Public"),
            TEXT("Generated")
        );

    if (!IFileManager::Get().DirectoryExists(
        *GeneratedDirectory))
    {
        if (!IFileManager::Get().MakeDirectory(
            *GeneratedDirectory,
            true))
        {
            OutError = FString::Printf(
                TEXT(
                    "Could not create generated directory:\n%s"
                ),
                *GeneratedDirectory
            );

            return false;
        }
    }

    OutGeneratedPath =
        FPaths::Combine(
            GeneratedDirectory,
            TEXT("GeneratedNetworks.h")
        );

    FString Source;

    Source +=
        TEXT("#pragma once\n\n");

    Source +=
        TEXT("#include \"CoreMinimal.h\"\n\n");

    Source +=
        TEXT("/**\n");

    Source +=
        TEXT(" * Generated by PlayTradeX.\n");

    Source +=
        TEXT(" * Do not edit manually.\n");

    Source +=
        TEXT(" * Regenerate this file from Project Settings.\n");

    Source +=
        TEXT(" */\n");

    Source +=
        TEXT("namespace GeneratedNetworks\n");

    Source +=
        TEXT("{\n");

    const UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    for (
        const FPlayTradeXNetworkSettings& Network :
        SettingsObject->Networks)
    {
        const FString TrimmedId =
            Network.Id.TrimStartAndEnd();

        const FString Identifier =
            ToCppIdentifier(TrimmedId);

        const FString EscapedId =
            EscapeCppString(TrimmedId);

        Source += FString::Printf(
            TEXT(
                "    inline const FString %s = TEXT(\"%s\");\n"
            ),
            *Identifier,
            *EscapedId
        );
    }

    Source +=
        TEXT("}\n");

    if (!FFileHelper::SaveStringToFile(
        Source,
        *OutGeneratedPath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
    {
        OutError = FString::Printf(
            TEXT(
                "Could not write generated class:\n%s"
            ),
            *OutGeneratedPath
        );

        return false;
    }

    return true;
}

bool
FPlayTradeXSettingsCustomization::ContainsWalletAddress(
    const FString& Address) const
{
    if (!Settings.IsValid())
    {
        return false;
    }

    const FString NormalizedAddress =
        Address.TrimStartAndEnd();

    for (
        const FPlayTradeXWalletSettings& Wallet :
        Settings->Wallets)
    {
        if (Wallet.Address.Equals(
            NormalizedAddress,
            ESearchCase::IgnoreCase))
        {
            return true;
        }
    }

    return false;
}

FString
FPlayTradeXSettingsCustomization::CreateUniqueWalletId() const
{
    if (!Settings.IsValid())
    {
        return TEXT("wallet-1");
    }

    int32 Number = 1;

    while (true)
    {
        const FString Candidate =
            FString::Printf(
                TEXT("wallet-%d"),
                Number
            );

        bool bExists = false;

        for (
            const FPlayTradeXWalletSettings& Wallet :
            Settings->Wallets)
        {
            if (Wallet.Id.Equals(
                Candidate,
                ESearchCase::CaseSensitive))
            {
                bExists = true;
                break;
            }
        }

        if (!bExists)
        {
            return Candidate;
        }

        ++Number;
    }
}

FReply
FPlayTradeXSettingsCustomization::
OnGenerateWalletClicked()
{
    if (!Settings.IsValid())
    {
        return FReply::Handled();
    }

    TWeakObjectPtr<UPlayTradeXSettings> WeakSettings =
        Settings;

    IDetailLayoutBuilder* CurrentDetailBuilder =
        DetailBuilder;

    PlayTradeX::SDK::GenerateWallet(
        [WeakSettings, CurrentDetailBuilder](
            const PlayTradeX::SDK::GenerateWalletResponse&
            NativeResponse)
        {
            const bool bSuccess =
                NativeResponse.success;

            const int32 ErrorCode =
                NativeResponse.errorCode;

            const FString ErrorMessage =
                FString(
                    UTF8_TO_TCHAR(
                        NativeResponse.errorMessage.c_str()
                    )
                );

            const FString Address =
                FString(
                    UTF8_TO_TCHAR(
                        NativeResponse.wallet.address.c_str()
                    )
                );

            const FString PrivateKey =
                FString(
                    UTF8_TO_TCHAR(
                        NativeResponse.wallet.privateKey.c_str()
                    )
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakSettings,
                    CurrentDetailBuilder,
                    bSuccess,
                    ErrorCode,
                    ErrorMessage,
                    Address,
                    PrivateKey
                ]()
                {
                    if (!WeakSettings.IsValid())
                    {
                        return;
                    }

                    if (!bSuccess)
                    {
                        FMessageDialog::Open(
                            EAppMsgType::Ok,
                            FText::Format(
                                LOCTEXT(
                                    "GenerateWalletFailed",
                                    "PlayTradeX could not generate the wallet.\n\nError Code: {0}\n{1}"
                                ),
                                FText::AsNumber(
                                    ErrorCode
                                ),
                                FText::FromString(
                                    ErrorMessage
                                )
                            )
                        );

                        return;
                    }

                    if (Address.IsEmpty())
                    {
                        FMessageDialog::Open(
                            EAppMsgType::Ok,
                            LOCTEXT(
                                "GeneratedWalletAddressEmpty",
                                "PlayTradeX generated a wallet with an empty address."
                            )
                        );

                        return;
                    }

                    if (PrivateKey.IsEmpty())
                    {
                        FMessageDialog::Open(
                            EAppMsgType::Ok,
                            LOCTEXT(
                                "GeneratedWalletPrivateKeyEmpty",
                                "PlayTradeX generated a wallet with an empty private key."
                            )
                        );

                        return;
                    }

                    UPlayTradeXSettings* SettingsObject =
                        WeakSettings.Get();

                    for (
                        const FPlayTradeXWalletSettings& ExistingWallet :
                        SettingsObject->Wallets)
                    {
                        if (ExistingWallet.Address.Equals(
                            Address,
                            ESearchCase::IgnoreCase))
                        {
                            FMessageDialog::Open(
                                EAppMsgType::Ok,
                                LOCTEXT(
                                    "GeneratedWalletDuplicate",
                                    "This wallet address is already configured in PlayTradeX."
                                )
                            );

                            return;
                        }
                    }

                    int32 WalletNumber = 1;

                    FString WalletId;

                    while (true)
                    {
                        WalletId =
                            FString::Printf(
                                TEXT("wallet-%d"),
                                WalletNumber
                            );

                        bool bIdExists = false;

                        for (
                            const FPlayTradeXWalletSettings& ExistingWallet :
                            SettingsObject->Wallets)
                        {
                            if (ExistingWallet.Id.Equals(
                                WalletId,
                                ESearchCase::CaseSensitive))
                            {
                                bIdExists = true;
                                break;
                            }
                        }

                        if (!bIdExists)
                        {
                            break;
                        }

                        ++WalletNumber;
                    }

                    SettingsObject->Modify();

                    FPlayTradeXWalletSettings Wallet;

                    Wallet.Id =
                        MoveTemp(WalletId);

                    Wallet.Address =
                        Address;

                    Wallet.PrivateKey =
                        PrivateKey;

                    SettingsObject->Wallets.Add(
                        MoveTemp(Wallet)
                    );

                    SettingsObject->SaveConfig();
                    SettingsObject->UpdateDefaultConfigFile();

                    if (CurrentDetailBuilder != nullptr)
                    {
                        CurrentDetailBuilder->
                            ForceRefreshDetails();
                    }

                    const FString ShortAddress =
                        Address.Len() > 10
                        ? Address.Left(6) +
                        TEXT("...") +
                        Address.Right(4)
                        : Address;

                    UE_LOG(
                        LogTemp,
                        Log,
                        TEXT(
                            "PlayTradeX: Generated wallet %s."
                        ),
                        *ShortAddress
                    );

                    FMessageDialog::Open(
                        EAppMsgType::Ok,
                        FText::Format(
                            LOCTEXT(
                                "GenerateWalletSucceeded",
                                "Wallet generated successfully.\n\nID: {0}\nAddress: {1}"
                            ),
                            FText::FromString(
                                SettingsObject->
                                Wallets.Last().Id
                            ),
                            FText::FromString(
                                ShortAddress
                            )
                        )
                    );
                }
            );
        }
    );

    return FReply::Handled();
}

bool
FPlayTradeXSettingsCustomization::ValidateWallets(
    FString& OutError) const
{
    OutError.Empty();

    if (!Settings.IsValid())
    {
        OutError =
            TEXT("PlayTradeX settings are unavailable.");

        return false;
    }

    const UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    if (SettingsObject->Wallets.IsEmpty())
    {
        OutError =
            TEXT(
                "Configure at least one wallet before generating the class."
            );

        return false;
    }

    TSet<FString> WalletIds;
    TSet<FString> GeneratedIdentifiers;

    for (
        int32 Index = 0;
        Index < SettingsObject->Wallets.Num();
        ++Index)
    {
        const FPlayTradeXWalletSettings& Wallet =
            SettingsObject->Wallets[Index];

        const FString TrimmedId =
            Wallet.Id.TrimStartAndEnd();

        if (TrimmedId.IsEmpty())
        {
            OutError = FString::Printf(
                TEXT(
                    "Wallet %d has an empty ID."
                ),
                Index + 1
            );

            return false;
        }

        if (WalletIds.Contains(TrimmedId))
        {
            OutError = FString::Printf(
                TEXT(
                    "Duplicate wallet ID: %s"
                ),
                *TrimmedId
            );

            return false;
        }

        WalletIds.Add(
            TrimmedId
        );

        const FString Identifier =
            ToCppIdentifier(
                TrimmedId
            );

        if (Identifier.IsEmpty())
        {
            OutError = FString::Printf(
                TEXT(
                    "Wallet ID '%s' cannot be converted to a valid C++ identifier."
                ),
                *TrimmedId
            );

            return false;
        }

        if (GeneratedIdentifiers.Contains(
            Identifier))
        {
            OutError = FString::Printf(
                TEXT(
                    "Wallet ID '%s' generates duplicate C++ identifier '%s'."
                ),
                *TrimmedId,
                *Identifier
            );

            return false;
        }

        GeneratedIdentifiers.Add(
            Identifier
        );
    }

    return true;
}

bool
FPlayTradeXSettingsCustomization::GenerateWalletsClass(
    FString& OutGeneratedPath,
    FString& OutError) const
{
    OutGeneratedPath.Empty();
    OutError.Empty();

    if (!Settings.IsValid())
    {
        OutError =
            TEXT("PlayTradeX settings are unavailable.");

        return false;
    }

    const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );

    if (!Plugin.IsValid())
    {
        OutError =
            TEXT(
                "Could not locate the PlayTradeX plugin."
            );

        return false;
    }

    const FString GeneratedDirectory =
        FPaths::Combine(
            Plugin->GetBaseDir(),
            TEXT("Source"),
            TEXT("PlayTradeX"),
            TEXT("Public"),
            TEXT("Generated")
        );

    if (!IFileManager::Get().DirectoryExists(
        *GeneratedDirectory))
    {
        if (!IFileManager::Get().MakeDirectory(
            *GeneratedDirectory,
            true))
        {
            OutError = FString::Printf(
                TEXT(
                    "Could not create generated directory:\n%s"
                ),
                *GeneratedDirectory
            );

            return false;
        }
    }

    OutGeneratedPath =
        FPaths::Combine(
            GeneratedDirectory,
            TEXT("GeneratedWallets.h")
        );

    FString Source;

    Source +=
        TEXT("#pragma once\n\n");

    Source +=
        TEXT("#include \"CoreMinimal.h\"\n\n");

    Source +=
        TEXT("/**\n");

    Source +=
        TEXT(" * Generated by PlayTradeX.\n");

    Source +=
        TEXT(" * Contains wallet IDs only.\n");

    Source +=
        TEXT(" * Wallet addresses and private keys are never generated here.\n");

    Source +=
        TEXT(" * Do not edit manually.\n");

    Source +=
        TEXT(" */\n");

    Source +=
        TEXT("namespace GeneratedWallets\n");

    Source +=
        TEXT("{\n");

    const UPlayTradeXSettings* SettingsObject =
        Settings.Get();

    for (
        const FPlayTradeXWalletSettings& Wallet :
        SettingsObject->Wallets)
    {
        const FString TrimmedId =
            Wallet.Id.TrimStartAndEnd();

        const FString Identifier =
            ToCppIdentifier(
                TrimmedId
            );

        const FString EscapedId =
            EscapeCppString(
                TrimmedId
            );

        Source += FString::Printf(
            TEXT(
                "    inline const FString %s = TEXT(\"%s\");\n"
            ),
            *Identifier,
            *EscapedId
        );
    }

    Source +=
        TEXT("}\n");

    if (!FFileHelper::SaveStringToFile(
        Source,
        *OutGeneratedPath,
        FFileHelper::EEncodingOptions::
        ForceUTF8WithoutBOM))
    {
        OutError = FString::Printf(
            TEXT(
                "Could not write generated class:\n%s"
            ),
            *OutGeneratedPath
        );

        return false;
    }

    return true;
}

FReply
FPlayTradeXSettingsCustomization::
OnGenerateWalletsClassClicked()
{
    FString ValidationError;

    if (!ValidateWallets(
        ValidationError))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::Format(
                LOCTEXT(
                    "GenerateWalletsValidationFailed",
                    "Cannot generate the wallet class.\n\n{0}"
                ),
                FText::FromString(
                    ValidationError
                )
            )
        );

        return FReply::Handled();
    }

    FString GeneratedPath;
    FString GenerationError;

    if (!GenerateWalletsClass(
        GeneratedPath,
        GenerationError))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::Format(
                LOCTEXT(
                    "GenerateWalletsFailed",
                    "Failed to generate the wallet class.\n\n{0}"
                ),
                FText::FromString(
                    GenerationError
                )
            )
        );

        return FReply::Handled();
    }

    FMessageDialog::Open(
        EAppMsgType::Ok,
        FText::Format(
            LOCTEXT(
                "GenerateWalletsSucceeded",
                "Generated wallet class successfully.\n\n{0}"
            ),
            FText::FromString(
                GeneratedPath
            )
        )
    );

    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: Generated wallet class: %s"
        ),
        *GeneratedPath
    );

    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE