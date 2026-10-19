#pragma once

#include "CoreMinimal.h"
#include "PlayTradeXUnrealTypes.generated.h"


// ============================================================================
// Wallet
// ============================================================================

UENUM(BlueprintType)
enum class EPlayTradeXWalletSource : uint8
{
    Identity UMETA(DisplayName = "Identity"),
    External UMETA(DisplayName = "External")
};


USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXGeneratedWallet
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString Address;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString PublicKey;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString PrivateKey;
};


USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXGenerateWalletResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    bool Success = false;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FPlayTradeXGeneratedWallet Wallet;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString ErrorMessage;
};


// ============================================================================
// Network
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXNetworkConfig
{
    GENERATED_BODY()

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "PlayTradeX|Network"
    )
    FString Id;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "PlayTradeX|Network"
    )
    int64 ChainId = 0;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "PlayTradeX|Network"
    )
    TArray<FString> RpcEndpoints;
};


// ============================================================================
// Execution Context
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXExecutionContext
{
    GENERATED_BODY()

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "PlayTradeX|Execution"
    )
    FString NetworkId;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "PlayTradeX|Execution"
    )
    EPlayTradeXWalletSource WalletSource =
        EPlayTradeXWalletSource::Identity;

    /*
     * Required only when WalletSource is External.
     *
     * This contains sensitive wallet material and should never be logged.
     */
    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "PlayTradeX|Execution"
    )
    FString ExternalPrivateKey;
};


// ============================================================================
// Initialization
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXInitializeResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Initialization")
    bool Initialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Initialization")
    FString Error;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Initialization")
    FString WalletAddress;
};


// ============================================================================
// Base Response
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXBaseResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Response")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Response")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Response")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Response")
    FString ErrorMessage;
};


// ============================================================================
// Native Balance
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXNativeBalanceResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Balance")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Balance")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Balance")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Balance")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Balance")
    FString Balance;
};


// ============================================================================
// Human Readable ABI
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXHumanReadableAbiResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|ABI")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|ABI")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|ABI")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|ABI")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|ABI")
    FString Abi;
};


// ============================================================================
// Transaction Response
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXTransactionResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString TransactionHash;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Receipt;
};


// ============================================================================
// Contract Read
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXContractReadResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Contract")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Contract")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Contract")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Contract")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Contract")
    FString Data;
};


// ============================================================================
// Wallet Export
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXWalletExportResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString WalletAddress;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString FilePath;
};


// ============================================================================
// Wallet Import
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXWalletImportResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    int32 Success = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    int32 ErrorCode = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString Body;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Wallet")
    FString WalletAddress;
};


// ============================================================================
// Prepared Transaction
// ============================================================================

USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXPreparedTransaction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString NetworkId;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    int64 ChainId = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString FromAddress;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Rpc;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString ContractAddress;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Abi;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString FunctionName;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Params;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Value;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    int64 GasLimit = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString BaseFeePerGas;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString MaxPriorityFeePerGas;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString MaxFeePerGas;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString EstimatedMaxNetworkFee;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    bool SimulationSucceeded = false;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString SimulationError;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    bool CanSubmit = false;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString PreparationError;
};


// ============================================================================
// Transaction Events
// ============================================================================

UENUM(BlueprintType)
enum class EPlayTradeXTransactionEventStatus : uint8
{
    Confirmed UMETA(DisplayName = "Confirmed"),
    Reverted UMETA(DisplayName = "Reverted")
};


USTRUCT(BlueprintType)
struct PLAYTRADEX_API FPlayTradeXTransactionEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString TransactionId;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString NetworkId;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    int64 ChainId = 0;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString TransactionHash;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    EPlayTradeXTransactionEventStatus Status =
        EPlayTradeXTransactionEventStatus::Confirmed;

    UPROPERTY(BlueprintReadOnly, Category = "PlayTradeX|Transaction")
    FString Receipt;
};