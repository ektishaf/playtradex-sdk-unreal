#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;
class UPlayTradeXSettings;

struct FPlayTradeXTestnetPreset
{
    FString Id;
    FString NetworkName;
    FString Ecosystem;

    int64 ChainId = 0;

    FString Symbol;

    TArray<FString> RpcUrls;

    FString BlockExplorerUrl;

    bool bIsTestnet = true;
};

class FPlayTradeXSettingsCustomization :
    public IDetailCustomization
{
public:

    static TSharedRef<IDetailCustomization> MakeInstance();

    virtual void CustomizeDetails(
        IDetailLayoutBuilder& DetailBuilder
    ) override;

private:

    TSharedRef<SWidget> BuildTestnetMenu();

    bool LoadTestnetCatalog(
        TArray<FPlayTradeXTestnetPreset>& OutPresets
    ) const;

    bool IsPresetConfigured(
        const FPlayTradeXTestnetPreset& Preset
    ) const;

    void AddTestnet(
        FPlayTradeXTestnetPreset Preset
    );

    FReply OnAddAllTestnetsClicked();

    FReply OnRemoveTestnetsClicked();

    FReply OnGenerateNetworksClassClicked();

    bool ValidateNetworks(
        FString& OutError
    ) const;

    bool GenerateNetworksClass(
        FString& OutGeneratedPath,
        FString& OutError
    ) const;

    FString ToCppIdentifier(
        const FString& Value
    ) const;

    bool IsCppKeyword(
        const FString& Value
    ) const;

    FString EscapeCppString(
        const FString& Value
    ) const;

    void SaveSettings();

    FReply OnGenerateWalletClicked();

    FReply OnGenerateWalletsClassClicked();

    bool ContainsWalletAddress(
        const FString& Address
    ) const;

    FString CreateUniqueWalletId() const;

    bool ValidateWallets(
        FString& OutError
    ) const;

    bool GenerateWalletsClass(
        FString& OutGeneratedPath,
        FString& OutError
    ) const;

private:

    TWeakObjectPtr<UPlayTradeXSettings> Settings;

    IDetailLayoutBuilder* DetailBuilder = nullptr;
};