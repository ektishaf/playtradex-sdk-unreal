#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PlayTradeXUnrealTypes.h"
#include "PlayTradeXDelegates.h"

#include "PlayTradeXSubsystem.generated.h"

// ============================================================
// Blueprint Request Delegates
// ============================================================

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXInitializeDynamicDelegate,
    const FPlayTradeXInitializeResult&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXGenerateWalletDynamicDelegate,
    const FPlayTradeXGenerateWalletResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXNativeBalanceDynamicDelegate,
    const FPlayTradeXNativeBalanceResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXTransactionDynamicDelegate,
    const FPlayTradeXTransactionResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXHumanReadableAbiDynamicDelegate,
    const FPlayTradeXHumanReadableAbiResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXContractReadDynamicDelegate,
    const FPlayTradeXContractReadResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXWalletExportDynamicDelegate,
    const FPlayTradeXWalletExportResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXWalletImportDynamicDelegate,
    const FPlayTradeXWalletImportResponse&,
    Result
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXTransactionConsentDynamicDelegate,
    const FPlayTradeXPreparedTransaction&,
    Transaction
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
    FPlayTradeXTransactionEventDynamicDelegate,
    const FPlayTradeXTransactionEvent&,
    Event
);


// ============================================================
// Blueprint Lifecycle Events
// ============================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FPlayTradeXReadyDynamicEvent
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FPlayTradeXNotReadyDynamicEvent
);

UCLASS()
class PLAYTRADEX_API UPlayTradeXSubsystem :
    public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    // ============================================================
    // Subsystem Lifecycle
    // ============================================================

    virtual void Initialize(
        FSubsystemCollectionBase& Collection
    ) override;

    virtual void Deinitialize() override;


    // ============================================================
    // Initialization
    // C++
    // ============================================================

    void InitializePlayTradeX(
        FPlayTradeXInitializeDelegate Callback
    );

    void InitializePlayTradeXWithConfiguration(
        const FString& StoragePath,
        const TArray<FPlayTradeXNetworkConfig>& Networks,
        FPlayTradeXInitializeDelegate Callback
    );

    void ShutdownPlayTradeX();

    bool IsInitialized() const;

    bool IsReady() const;

    bool IsInitializing() const;

    FPlayTradeXReadyEvent OnReadyNative;

    FPlayTradeXNotReadyEvent OnNotReadyNative;

    UPROPERTY(BlueprintAssignable, Category = "PlayTradeX|Initialization")
    FPlayTradeXReadyDynamicEvent OnReady;

    UPROPERTY(BlueprintAssignable, Category = "PlayTradeX|Initialization")
    FPlayTradeXNotReadyDynamicEvent OnNotReady;


    // ============================================================
    // Initialization
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Initialization",
        meta = (
            DisplayName = "Initialize PlayTradeX"
            )
    )
    void K2_InitializePlayTradeX(
        FPlayTradeXInitializeDynamicDelegate Callback
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Initialization",
        meta = (
            DisplayName =
            "Initialize PlayTradeX With Configuration"
            )
    )
    void K2_InitializePlayTradeXWithConfiguration(
        const FString& StoragePath,
        const TArray<FPlayTradeXNetworkConfig>& Networks,
        FPlayTradeXInitializeDynamicDelegate Callback
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Initialization",
        meta = (
            DisplayName = "Shutdown PlayTradeX"
            )
    )
    void K2_ShutdownPlayTradeX();

    UFUNCTION(
        BlueprintPure,
        Category = "PlayTradeX|Initialization",
        meta = (
            DisplayName = "Is PlayTradeX Initialized"
            )
    )
    bool K2_IsInitialized() const;

    UFUNCTION(
        BlueprintPure,
        Category = "PlayTradeX|Initialization",
        meta = (
            DisplayName = "Is PlayTradeX Ready"
            )
    )
    bool K2_IsReady() const;

    UFUNCTION(
        BlueprintPure,
        Category = "PlayTradeX|Initialization",
        meta = (
            DisplayName = "Is PlayTradeX Initializing"
            )
    )
    bool K2_IsInitializing() const;


    // ============================================================
    // Generate Wallet
    // C++
    // ============================================================

    void GenerateWallet(
        FPlayTradeXGenerateWalletDelegate Callback
    );


    // ============================================================
    // Generate Wallet
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Wallet",
        meta = (
            DisplayName = "Generate Wallet"
            )
    )
    void K2_GenerateWallet(
        FPlayTradeXGenerateWalletDynamicDelegate Callback
    );


    // ============================================================
    // Transaction Events
    // C++
    // ============================================================

    FPlayTradeXTransactionEventListenerId
        SubscribeTransactionEvents(
            FPlayTradeXTransactionEventDelegate Callback
        );

    bool UnsubscribeTransactionEvents(
        FPlayTradeXTransactionEventListenerId ListenerId
    );


    // ============================================================
    // Transaction Events
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction Events",
        meta = (
            DisplayName = "Subscribe Transaction Events"
            )
    )
    int64 K2_SubscribeTransactionEvents(
        FPlayTradeXTransactionEventDynamicDelegate Callback
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction Events",
        meta = (
            DisplayName = "Unsubscribe Transaction Events"
            )
    )
    bool K2_UnsubscribeTransactionEvents(
        int64 ListenerId
    );


    // ============================================================
    // Transaction Consent
    // C++
    // ============================================================

    void SetTransactionConsentCallback(
        FPlayTradeXTransactionConsentDelegate Callback
    );

    bool ApproveTransaction(
        const FString& TransactionId
    );

    bool DenyTransaction(
        const FString& TransactionId
    );


    // ============================================================
    // Transaction Consent
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction Consent",
        meta = (
            DisplayName =
            "Set Transaction Consent Callback"
            )
    )
    void K2_SetTransactionConsentCallback(
        FPlayTradeXTransactionConsentDynamicDelegate Callback
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction Consent",
        meta = (
            DisplayName =
            "Clear Transaction Consent Callback"
            )
    )
    void K2_ClearTransactionConsentCallback();

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction Consent",
        meta = (
            DisplayName = "Approve Transaction"
            )
    )
    bool K2_ApproveTransaction(
        const FString& TransactionId
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction Consent",
        meta = (
            DisplayName = "Deny Transaction"
            )
    )
    bool K2_DenyTransaction(
        const FString& TransactionId
    );


    // ============================================================
    // Native Balance
    // C++
    // ============================================================

    void GetNativeBalance(
        const FString& NetworkId,
        FPlayTradeXNativeBalanceDelegate Callback,
        bool bSequential = false
    );

    void GetNativeBalance(
        const FString& NetworkId,
        const FString& Address,
        FPlayTradeXNativeBalanceDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Native Balance
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Blockchain",
        meta = (
            DisplayName = "Get Native Balance"
            )
    )
    void K2_GetNativeBalance(
        const FString& NetworkId,
        FPlayTradeXNativeBalanceDynamicDelegate Callback,
        bool bSequential = false
    );

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Blockchain",
        meta = (
            DisplayName =
            "Get Native Balance For Address"
            )
    )
    void K2_GetNativeBalanceForAddress(
        const FString& NetworkId,
        const FString& Address,
        FPlayTradeXNativeBalanceDynamicDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Send ETH
    // C++
    // ============================================================

    void SendEth(
        const FPlayTradeXExecutionContext& Context,
        const FString& To,
        const FString& Amount,
        FPlayTradeXTransactionDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Send ETH
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Transaction",
        meta = (
            DisplayName = "Send ETH"
            )
    )
    void K2_SendEth(
        const FPlayTradeXExecutionContext& Context,
        const FString& To,
        const FString& Amount,
        FPlayTradeXTransactionDynamicDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Write
    // C++
    // ============================================================

    void Write(
        const FPlayTradeXExecutionContext& Context,
        const FString& ContractAddress,
        const FString& Abi,
        const FString& FunctionName,
        const FString& Params,
        FPlayTradeXTransactionDelegate Callback,
        const FString& Value = TEXT("0"),
        bool bSequential = false
    );


    // ============================================================
    // Write
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Contract",
        meta = (
            DisplayName = "Write Contract"
            )
    )
    void K2_Write(
        const FPlayTradeXExecutionContext& Context,
        const FString& ContractAddress,
        const FString& Abi,
        const FString& FunctionName,
        const FString& Params,
        FPlayTradeXTransactionDynamicDelegate Callback,
        const FString& Value = TEXT("0"),
        bool bSequential = false
    );


    // ============================================================
    // Read
    // C++
    // ============================================================

    void Read(
        const FString& NetworkId,
        const FString& ContractAddress,
        const FString& Abi,
        const FString& FunctionName,
        const FString& Params,
        FPlayTradeXContractReadDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Read
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Contract",
        meta = (
            DisplayName = "Read Contract"
            )
    )
    void K2_Read(
        const FString& NetworkId,
        const FString& ContractAddress,
        const FString& Abi,
        const FString& FunctionName,
        const FString& Params,
        FPlayTradeXContractReadDynamicDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Human Readable ABI
    // C++
    // ============================================================

    void HumanReadableAbi(
        const FString& Abi,
        bool bMinimal,
        FPlayTradeXHumanReadableAbiDelegate Callback
    );


    // ============================================================
    // Human Readable ABI
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|ABI",
        meta = (
            DisplayName = "Human Readable ABI"
            )
    )
    void K2_HumanReadableAbi(
        const FString& Abi,
        bool bMinimal,
        FPlayTradeXHumanReadableAbiDynamicDelegate Callback
    );


    // ============================================================
    // Export Wallet
    // C++
    // ============================================================

    void ExportWallet(
        const FString& Password,
        FPlayTradeXWalletExportDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Export Wallet
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Wallet",
        meta = (
            DisplayName = "Export Wallet"
            )
    )
    void K2_ExportWallet(
        const FString& Password,
        FPlayTradeXWalletExportDynamicDelegate Callback,
        bool bSequential = false
    );


    // ============================================================
    // Import Wallet
    // C++
    // ============================================================

    void ImportWallet(
        const FString& Password,
        FPlayTradeXWalletImportDelegate Callback
    );


    // ============================================================
    // Import Wallet
    // Blueprint
    // ============================================================

    UFUNCTION(
        BlueprintCallable,
        Category = "PlayTradeX|Wallet",
        meta = (
            DisplayName = "Import Wallet"
            )
    )
    void K2_ImportWallet(
        const FString& Password,
        FPlayTradeXWalletImportDynamicDelegate Callback
    );

private:

    // ============================================================
    // Internal Initialization
    // ============================================================

    void InitializeNative(
        const FString& StoragePath,
        const TArray<FPlayTradeXNetworkConfig>& Networks,
        FPlayTradeXInitializeDelegate Callback
    );

    FString ResolveStoragePath(
        const FString& ConfiguredStoragePath
    ) const;

    void CompleteInitialization(
        uint64 Generation,
        const FPlayTradeXInitializeResult& Result,
        FPlayTradeXInitializeDelegate Callback
    );

    void BroadcastReady();

    void BroadcastNotReady();

    bool bInitializationInProgress = false;
    bool bReady = false;
    bool bDeinitializing = false;
    uint64 InitializationGeneration = 0;
};
