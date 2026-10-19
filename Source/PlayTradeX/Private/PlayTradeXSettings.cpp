#include "PlayTradeXSettings.h"

#define LOCTEXT_NAMESPACE "PlayTradeXSettings"


UPlayTradeXSettings::UPlayTradeXSettings()
{
    StoragePath = TEXT("");
}

const UPlayTradeXSettings* UPlayTradeXSettings::Get()
{
    return GetDefault<UPlayTradeXSettings>();
}

FPlayTradeXNetworkSettings UPlayTradeXSettings::GetNetworkById(const FString& networkId)
{
    const UPlayTradeXSettings* settings = UPlayTradeXSettings::Get();
    if (!settings)
    {
        return {};
    }

    for (const FPlayTradeXNetworkSettings& element : settings->Networks)
    {
        if (element.Id.Equals(networkId, ESearchCase::IgnoreCase))
        {
            return element;
        }
    }

    return {};
}

FPlayTradeXWalletSettings UPlayTradeXSettings::GetWalletById(const FString& walletId)
{
    const UPlayTradeXSettings* settings = UPlayTradeXSettings::Get();
    if (!settings)
    {
        return {};
    }

    for (const FPlayTradeXWalletSettings& element : settings->Wallets)
    {
        if (element.Id.Equals(walletId, ESearchCase::IgnoreCase))
        {
            return element;
        }
    }

    return {};
}

FName UPlayTradeXSettings::GetCategoryName() const
{
    return TEXT("Plugins");
}


#if WITH_EDITOR

FText UPlayTradeXSettings::GetSectionText() const
{
    return LOCTEXT(
        "PlayTradeXSettingsSection",
        "PlayTradeX"
    );
}


FText UPlayTradeXSettings::GetSectionDescription() const
{
    return LOCTEXT(
        "PlayTradeXSettingsDescription",
        "Configure blockchain networks and application managed wallets used by PlayTradeX."
    );
}


void UPlayTradeXSettings::PostEditChangeProperty(
    FPropertyChangedEvent& PropertyChangedEvent
)
{
    Super::PostEditChangeProperty(
        PropertyChangedEvent
    );


    // Persist Project Settings changes into the project's
    // default configuration.
    //
    // This also covers changes made directly through the
    // Project Settings details panel, including nested
    // Networks, Wallets and RPC URL array values.

    UpdateDefaultConfigFile();
}

#endif


#undef LOCTEXT_NAMESPACE