#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PlayTradeXSettings.generated.h"

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXNetworkSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network")
    FString Id;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network")
    FString NetworkName;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Network",
        meta = (ClampMin = "1")
    )
    int64 ChainId = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network")
    FString Symbol;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network")
    TArray<FString> RpcUrls;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Network")
    FString BlockExplorerUrl;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Network")
    bool bIsTestnet = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Network")
    FString PresetId;
};


USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXWalletSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wallet")
    FString Id;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wallet")
    FString Address;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Wallet",
        meta = (PasswordField = "true")
    )
    FString PrivateKey;
};


UCLASS(
    Config = Game,
    DefaultConfig,
    meta = (DisplayName = "PlayTradeX")
)
class PLAYTRADEX_API UPlayTradeXSettings :
    public UDeveloperSettings
{
    GENERATED_BODY()

public:

    UPlayTradeXSettings();


    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Config,
        Category = "Initialization",
        meta = (DisplayName = "Auto Initialize")
    )
    bool bAutoInitialize = true;


    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "Storage",
        meta = (DisplayName = "Storage Path")
    )
    FString StoragePath;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Config,
        Category = "Networks",
        meta = (TitleProperty = "Id")
    )
    TArray<FPlayTradeXNetworkSettings> Networks;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Config,
        Category = "Wallets",
        meta = (TitleProperty = "Id")
    )
    TArray<FPlayTradeXWalletSettings> Wallets;


    virtual FName GetCategoryName() const override;

    UFUNCTION(
        BlueprintPure,
        Category = "PlayTradeX|Settings",
        meta = (DisplayName = "Get PlayTradeX Settings")
    )
    static const UPlayTradeXSettings* Get();

    UFUNCTION(
        BlueprintPure,
        Category = "PlayTradeX|Settings",
        meta = (DisplayName = "Get PlayTradeX Network")
    )
    static FPlayTradeXNetworkSettings GetNetworkById(const FString& networkId);

    UFUNCTION(
        BlueprintPure,
        Category = "PlayTradeX|Settings",
        meta = (DisplayName = "Get PlayTradeX Wallet")
    )
    static FPlayTradeXWalletSettings GetWalletById(const FString& walletId);


#if WITH_EDITOR

    virtual FText GetSectionText() const override;

    virtual FText GetSectionDescription() const override;

    virtual void PostEditChangeProperty(
        FPropertyChangedEvent& PropertyChangedEvent
    ) override;

#endif
};