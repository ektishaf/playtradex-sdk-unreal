#include "PlayTradeXSubsystem.h"

#include "Async/Async.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

#include "PlayTradeX.h"
#include "PlayTradeXSettings.h"

#include <string>
#include <utility>
#include <vector>

#if PLATFORM_ANDROID
#include "PlayTradeXAndroidBridge.h"
#endif

#include "PlayTradeXFilePicker.h"

namespace
{
    // ============================================================
    // String Conversion
    // ============================================================

    std::string ToStdString(
        const FString& Value)
    {
        return std::string(
            TCHAR_TO_UTF8(*Value)
        );
    }


    FString ToFString(
        const std::string& Value)
    {
        return FString(
            UTF8_TO_TCHAR(
                Value.c_str()
            )
        );
    }


    // ============================================================
    // Network Conversion
    // ============================================================

    bool ConvertNetworkConfig(
        const FPlayTradeXNetworkConfig& Source,
        PlayTradeX::SDK::NetworkConfig& Target)
    {
        if (Source.ChainId <= 0)
        {
            return false;
        }

        Target.id =
            ToStdString(Source.Id);

        Target.chainId =
            static_cast<uint64_t>(
                Source.ChainId
                );

        Target.rpcEndpoints.clear();

        Target.rpcEndpoints.reserve(
            Source.RpcEndpoints.Num()
        );

        for (
            const FString& Rpc :
            Source.RpcEndpoints)
        {
            Target.rpcEndpoints.emplace_back(
                ToStdString(Rpc)
            );
        }

        return true;
    }


    // ============================================================
    // Execution Context Conversion
    // ============================================================

    PlayTradeX::SDK::ExecutionContext
        ConvertExecutionContext(
            const FPlayTradeXExecutionContext& Source)
    {
        PlayTradeX::SDK::ExecutionContext Target;

        Target.networkId =
            ToStdString(
                Source.NetworkId
            );

        Target.walletSource =
            Source.WalletSource ==
            EPlayTradeXWalletSource::External
            ? PlayTradeX::SDK::WalletSource::External
            : PlayTradeX::SDK::WalletSource::Identity;

        Target.externalPrivateKey =
            ToStdString(
                Source.ExternalPrivateKey
            );

        return Target;
    }


    // ============================================================
    // Initialize Result
    // ============================================================

    FPlayTradeXInitializeResult
        ConvertInitializeResult(
            const PlayTradeX::SDK::InitializeResult& Source)
    {
        FPlayTradeXInitializeResult Target;

        Target.Initialized =
            Source.initialized;

        Target.Error =
            ToFString(Source.error);

        Target.WalletAddress =
            ToFString(
                Source.walletAddress
            );

        return Target;
    }


    // ============================================================
    // Generate Wallet
    // ============================================================

    FPlayTradeXGenerateWalletResponse
        ConvertGenerateWalletResponse(
            const PlayTradeX::SDK::GenerateWalletResponse& Source)
    {
        FPlayTradeXGenerateWalletResponse Target;

        Target.Success =
            Source.success;

        Target.ErrorCode =
            Source.errorCode;

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.Wallet.Address =
            ToFString(
                Source.wallet.address
            );

        Target.Wallet.PublicKey =
            ToFString(
                Source.wallet.publicKey
            );

        Target.Wallet.PrivateKey =
            ToFString(
                Source.wallet.privateKey
            );

        return Target;
    }


    // ============================================================
    // Native Balance
    // ============================================================

    FPlayTradeXNativeBalanceResponse
        ConvertNativeBalanceResponse(
            const PlayTradeX::SDK::NativeBalanceResponse& Source)
    {
        FPlayTradeXNativeBalanceResponse Target;

        Target.Success =
            Source.success != 0;

        Target.ErrorCode =
            Source.errorCode;

        Target.Body =
            ToFString(Source.body);

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.Balance =
            ToFString(Source.balance);

        return Target;
    }


    // ============================================================
    // Transaction Response
    // ============================================================

    FPlayTradeXTransactionResponse
        ConvertTransactionResponse(
            const PlayTradeX::SDK::TransactionResponse& Source)
    {
        FPlayTradeXTransactionResponse Target;

        Target.Success =
            Source.success != 0;

        Target.ErrorCode =
            Source.errorCode;

        Target.Body =
            ToFString(Source.body);

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.TransactionHash =
            ToFString(
                Source.transactionHash
            );

        Target.Receipt =
            ToFString(Source.receipt);

        return Target;
    }


    // ============================================================
    // Human Readable ABI
    // ============================================================

    FPlayTradeXHumanReadableAbiResponse
        ConvertHumanReadableAbiResponse(
            const PlayTradeX::SDK::HumanReadableAbiResponse& Source)
    {
        FPlayTradeXHumanReadableAbiResponse Target;

        Target.Success =
            Source.success != 0;

        Target.ErrorCode =
            Source.errorCode;

        Target.Body =
            ToFString(Source.body);

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.Abi =
            ToFString(Source.abi);

        return Target;
    }


    // ============================================================
    // Contract Read
    // ============================================================

    FPlayTradeXContractReadResponse
        ConvertContractReadResponse(
            const PlayTradeX::SDK::ContractReadResponse& Source)
    {
        FPlayTradeXContractReadResponse Target;

        Target.Success =
            Source.success != 0;

        Target.ErrorCode =
            Source.errorCode;

        Target.Body =
            ToFString(Source.body);

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.Data =
            ToFString(Source.data);

        return Target;
    }


    // ============================================================
    // Wallet Export
    // ============================================================

    FPlayTradeXWalletExportResponse
        ConvertWalletExportResponse(
            const PlayTradeX::SDK::WalletExportResponse& Source)
    {
        FPlayTradeXWalletExportResponse Target;

        Target.Success =
            Source.success != 0;

        Target.ErrorCode =
            Source.errorCode;

        Target.Body =
            ToFString(Source.body);

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.WalletAddress =
            ToFString(
                Source.walletAddress
            );

        Target.FilePath =
            ToFString(
                Source.filePath
            );

        return Target;
    }


    // ============================================================
    // Wallet Import
    // ============================================================

    FPlayTradeXWalletImportResponse
        ConvertWalletImportResponse(
            const PlayTradeX::SDK::WalletImportResponse& Source)
    {
        FPlayTradeXWalletImportResponse Target;

        Target.Success =
            Source.success != 0;

        Target.ErrorCode =
            Source.errorCode;

        Target.Body =
            ToFString(Source.body);

        Target.ErrorMessage =
            ToFString(
                Source.errorMessage
            );

        Target.WalletAddress =
            ToFString(
                Source.walletAddress
            );

        return Target;
    }


    // ============================================================
    // Prepared Transaction
    // ============================================================

    FPlayTradeXPreparedTransaction
        ConvertPreparedTransaction(
            const PlayTradeX::SDK::PreparedTransaction& Source)
    {
        FPlayTradeXPreparedTransaction Target;

        Target.Id =
            ToFString(Source.id);

        Target.NetworkId =
            ToFString(
                Source.networkId
            );

        Target.ChainId =
            static_cast<int64>(
                Source.chainId
                );

        Target.FromAddress =
            ToFString(
                Source.fromAddress
            );

        Target.Rpc =
            ToFString(Source.rpc);

        Target.ContractAddress =
            ToFString(
                Source.contractAddress
            );

        Target.Abi =
            ToFString(Source.abi);

        Target.FunctionName =
            ToFString(
                Source.functionName
            );

        Target.Params =
            ToFString(Source.params);

        Target.Value =
            ToFString(Source.value);

        Target.GasLimit =
            static_cast<int64>(
                Source.gasLimit
                );

        Target.BaseFeePerGas =
            ToFString(
                Source.baseFeePerGas
            );

        Target.MaxPriorityFeePerGas =
            ToFString(
                Source.maxPriorityFeePerGas
            );

        Target.MaxFeePerGas =
            ToFString(
                Source.maxFeePerGas
            );

        Target.EstimatedMaxNetworkFee =
            ToFString(
                Source.estimatedMaxNetworkFee
            );

        Target.SimulationSucceeded =
            Source.simulationSucceeded;

        Target.SimulationError =
            ToFString(
                Source.simulationError
            );

        Target.CanSubmit =
            Source.canSubmit;

        Target.PreparationError =
            ToFString(
                Source.preparationError
            );

        return Target;
    }


    // ============================================================
    // Transaction Event
    // ============================================================

    FPlayTradeXTransactionEvent
        ConvertTransactionEvent(
            const PlayTradeX::SDK::TransactionEvent& Source)
    {
        FPlayTradeXTransactionEvent Target;

        Target.TransactionId =
            ToFString(
                Source.transactionId
            );

        Target.NetworkId =
            ToFString(
                Source.networkId
            );

        Target.ChainId =
            static_cast<int64>(
                Source.chainId
                );

        Target.TransactionHash =
            ToFString(
                Source.transactionHash
            );

        Target.Status =
            Source.status ==
            PlayTradeX::SDK::
            TransactionEventStatus::Confirmed
            ? EPlayTradeXTransactionEventStatus::Confirmed
            : EPlayTradeXTransactionEventStatus::Reverted;

        Target.Receipt =
            ToFString(
                Source.receipt
            );

        return Target;
    }
}


// ============================================================
// Subsystem Lifecycle
// ============================================================

void UPlayTradeXSubsystem::Initialize(
    FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    bDeinitializing = false;
    bInitializationInProgress = false;
    bReady = PlayTradeX::SDK::IsInitialized();

    const UPlayTradeXSettings* Settings =
        GetDefault<UPlayTradeXSettings>();

    if (Settings != nullptr && Settings->bAutoInitialize)
    {
        InitializePlayTradeX(
            FPlayTradeXInitializeDelegate()
        );
    }
}


void UPlayTradeXSubsystem::Deinitialize()
{
    bDeinitializing = true;

    ++InitializationGeneration;
    bInitializationInProgress = false;

    PlayTradeX::SDK::
        SetTransactionConsentCallback({});

    // Shutdown is intentionally unconditional. The native SDK owns the
    // synchronization required to stop an initialization that is still in
    // progress as well as a fully initialized runtime.
    PlayTradeX::SDK::Shutdown();

    if (bReady)
    {
        bReady = false;
        BroadcastNotReady();
    }

    Super::Deinitialize();
}


// ============================================================
// Initialization
// C++
// ============================================================

void UPlayTradeXSubsystem::InitializePlayTradeX(
    FPlayTradeXInitializeDelegate Callback)
{
    const UPlayTradeXSettings* Settings =
        GetDefault<UPlayTradeXSettings>();

    if (Settings == nullptr)
    {
        FPlayTradeXInitializeResult Result;
        Result.Initialized = false;
        Result.Error =
            TEXT("PlayTradeX Project Settings are unavailable.");

        Callback.ExecuteIfBound(Result);
        BroadcastNotReady();
        return;
    }

    TArray<FPlayTradeXNetworkConfig> Networks;
    Networks.Reserve(Settings->Networks.Num());

    for (const FPlayTradeXNetworkSettings& Source : Settings->Networks)
    {
        FPlayTradeXNetworkConfig Target;
        Target.Id = Source.Id;
        Target.ChainId = Source.ChainId;
        Target.RpcEndpoints = Source.RpcUrls;
        Networks.Add(MoveTemp(Target));
    }

    InitializeNative(
        ResolveStoragePath(Settings->StoragePath),
        Networks,
        MoveTemp(Callback)
    );
}


void UPlayTradeXSubsystem::InitializePlayTradeXWithConfiguration(
    const FString& StoragePath,
    const TArray<FPlayTradeXNetworkConfig>& Networks,
    FPlayTradeXInitializeDelegate Callback)
{
    InitializeNative(
        ResolveStoragePath(StoragePath),
        Networks,
        MoveTemp(Callback)
    );
}


void UPlayTradeXSubsystem::InitializeNative(
    const FString& StoragePath,
    const TArray<FPlayTradeXNetworkConfig>& Networks,
    FPlayTradeXInitializeDelegate Callback)
{
    if (bDeinitializing)
    {
        FPlayTradeXInitializeResult Result;
        Result.Initialized = false;
        Result.Error = TEXT("PlayTradeX subsystem is deinitializing.");
        Callback.ExecuteIfBound(Result);
        return;
    }

    if (PlayTradeX::SDK::IsInitialized())
    {
        bInitializationInProgress = false;

        FPlayTradeXInitializeResult Result;
        Result.Initialized = true;
        Result.Error = TEXT("PlayTradeX SDK is already initialized.");

        if (!bReady)
        {
            bReady = true;
            BroadcastReady();
        }

        Callback.ExecuteIfBound(Result);
        return;
    }

    if (bInitializationInProgress)
    {
        FPlayTradeXInitializeResult Result;
        Result.Initialized = false;
        Result.Error = TEXT("PlayTradeX SDK initialization is already in progress.");
        Callback.ExecuteIfBound(Result);
        return;
    }

    auto FailInitialization =
        [this, &Callback](const FString& Error)
        {
            bInitializationInProgress = false;
            bReady = false;

            FPlayTradeXInitializeResult Result;
            Result.Initialized = false;
            Result.Error = Error;

            Callback.ExecuteIfBound(Result);
            BroadcastNotReady();
        };

    if (Networks.IsEmpty())
    {
        FailInitialization(
            TEXT("No PlayTradeX networks are configured.")
        );
        return;
    }

    if (StoragePath.IsEmpty())
    {
        FailInitialization(
            TEXT("PlayTradeX storage path could not be resolved.")
        );
        return;
    }

    if (!IFileManager::Get().DirectoryExists(*StoragePath))
    {
        if (!IFileManager::Get().MakeDirectory(*StoragePath, true))
        {
            FailInitialization(
                FString::Printf(
                    TEXT("Could not create PlayTradeX storage directory: %s"),
                    *StoragePath
                )
            );
            return;
        }
    }

    TSet<FString> NetworkIds;
    std::vector<PlayTradeX::SDK::NetworkConfig> NativeNetworks;
    NativeNetworks.reserve(Networks.Num());

    for (const FPlayTradeXNetworkConfig& Network : Networks)
    {
        const FString TrimmedId = Network.Id.TrimStartAndEnd();

        if (TrimmedId.IsEmpty())
        {
            FailInitialization(
                TEXT("A configured PlayTradeX network has an empty ID.")
            );
            return;
        }

        if (NetworkIds.Contains(TrimmedId))
        {
            FailInitialization(
                FString::Printf(
                    TEXT("Duplicate PlayTradeX network ID: '%s'."),
                    *TrimmedId
                )
            );
            return;
        }

        NetworkIds.Add(TrimmedId);

        if (Network.ChainId <= 0)
        {
            FailInitialization(
                FString::Printf(
                    TEXT("Network '%s' has an invalid Chain ID."),
                    *Network.Id
                )
            );
            return;
        }

        if (Network.RpcEndpoints.IsEmpty())
        {
            FailInitialization(
                FString::Printf(
                    TEXT("Network '%s' has no RPC endpoints."),
                    *Network.Id
                )
            );
            return;
        }

        for (const FString& Rpc : Network.RpcEndpoints)
        {
            if (Rpc.TrimStartAndEnd().IsEmpty())
            {
                FailInitialization(
                    FString::Printf(
                        TEXT("Network '%s' contains an empty RPC endpoint."),
                        *Network.Id
                    )
                );
                return;
            }
        }

        PlayTradeX::SDK::NetworkConfig NativeNetwork;

        if (!ConvertNetworkConfig(Network, NativeNetwork))
        {
            FailInitialization(
                FString::Printf(
                    TEXT("Network '%s' could not be converted."),
                    *Network.Id
                )
            );
            return;
        }

        NativeNetworks.emplace_back(std::move(NativeNetwork));
    }

    bInitializationInProgress = true;
    bReady = false;

    const uint64 Generation = ++InitializationGeneration;
    TWeakObjectPtr<UPlayTradeXSubsystem> WeakThis(this);
    
#if PLATFORM_ANDROID

    if (!FPlayTradeXAndroidBridge::Initialize())
    {
        FailInitialization(
            TEXT("Could not initialize PlayTradeX Android services.")
        );

        return;
    }

#endif
    
    PlayTradeX::SDK::Initialize(
        ToStdString(StoragePath),
        NativeNetworks,
        [
            WeakThis,
            Generation,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::InitializeResult& NativeResult
            ) mutable
        {
            FPlayTradeXInitializeResult Result =
                ConvertInitializeResult(NativeResult);

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Generation,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    WeakThis->CompleteInitialization(
                        Generation,
                        Result,
                        MoveTemp(Callback)
                    );
                }
            );
        }
    );
}


void UPlayTradeXSubsystem::CompleteInitialization(
    const uint64 Generation,
    const FPlayTradeXInitializeResult& Result,
    FPlayTradeXInitializeDelegate Callback)
{
    if (bDeinitializing || Generation != InitializationGeneration)
    {
        return;
    }

    bInitializationInProgress = false;

    const bool bWasReady = bReady;
    bReady = Result.Initialized && PlayTradeX::SDK::IsInitialized();

    Callback.ExecuteIfBound(Result);

    if (bReady)
    {
        if (!bWasReady)
        {
            BroadcastReady();
        }
    }
    else
    {
        BroadcastNotReady();
    }
}


void UPlayTradeXSubsystem::ShutdownPlayTradeX()
{
    ++InitializationGeneration;
    bInitializationInProgress = false;

    PlayTradeX::SDK::
        SetTransactionConsentCallback({});

    PlayTradeX::SDK::Shutdown();

    if (bReady)
    {
        bReady = false;
        BroadcastNotReady();
    }
}


bool UPlayTradeXSubsystem::IsInitialized() const
{
    return PlayTradeX::SDK::IsInitialized();
}


bool UPlayTradeXSubsystem::IsReady() const
{
    return bReady && PlayTradeX::SDK::IsInitialized();
}


bool UPlayTradeXSubsystem::IsInitializing() const
{
    return bInitializationInProgress;
}


void UPlayTradeXSubsystem::BroadcastReady()
{
    OnReadyNative.Broadcast();
    OnReady.Broadcast();
}


void UPlayTradeXSubsystem::BroadcastNotReady()
{
    OnNotReadyNative.Broadcast();
    OnNotReady.Broadcast();
}

FString UPlayTradeXSubsystem::ResolveStoragePath(
    const FString& ConfiguredPath) const
{
    FString ResolvedPath =
        ConfiguredPath.TrimStartAndEnd();

    // Match Unity behavior:
    // use the configured storage path when provided.
    if (!ResolvedPath.IsEmpty())
    {
        ResolvedPath =
            FPaths::ConvertRelativePathToFull(
                ResolvedPath
            );

        FPaths::NormalizeDirectoryName(
            ResolvedPath
        );

        return ResolvedPath;
    }

    // No path configured.
    // Use Unreal's persistent writable application directory.
    ResolvedPath =
        FPaths::ProjectPersistentDownloadDir();

    // Defensive fallback in case the platform does not provide
    // a persistent download directory.
    if (ResolvedPath.IsEmpty())
    {
        ResolvedPath =
            FPaths::ProjectSavedDir();
    }

    ResolvedPath =
        FPaths::ConvertRelativePathToFull(
            ResolvedPath
        );

    FPaths::NormalizeDirectoryName(
        ResolvedPath
    );

    return ResolvedPath;
}


// ============================================================
// Initialization
// Blueprint
// ============================================================

void
UPlayTradeXSubsystem::
K2_InitializePlayTradeX(
    FPlayTradeXInitializeDynamicDelegate Callback)
{
    InitializePlayTradeX(
        FPlayTradeXInitializeDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXInitializeResult& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    )
    );
}


void
UPlayTradeXSubsystem::
K2_InitializePlayTradeXWithConfiguration(
    const FString& StoragePath,
    const TArray<FPlayTradeXNetworkConfig>& Networks,
    FPlayTradeXInitializeDynamicDelegate Callback)
{
    InitializePlayTradeXWithConfiguration(
        StoragePath,
        Networks,
        FPlayTradeXInitializeDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXInitializeResult& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    )
    );
}


void
UPlayTradeXSubsystem::
K2_ShutdownPlayTradeX()
{
    ShutdownPlayTradeX();
}


bool
UPlayTradeXSubsystem::
K2_IsInitialized() const
{
    return IsInitialized();
}


bool
UPlayTradeXSubsystem::
K2_IsReady() const
{
    return IsReady();
}


bool
UPlayTradeXSubsystem::
K2_IsInitializing() const
{
    return IsInitializing();
}


// ============================================================
// Generate Wallet
// ============================================================

void UPlayTradeXSubsystem::GenerateWallet(
    FPlayTradeXGenerateWalletDelegate Callback)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::GenerateWallet(
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            GenerateWalletResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXGenerateWalletResponse Result =
                ConvertGenerateWalletResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        }
    );
}


void
UPlayTradeXSubsystem::
K2_GenerateWallet(
    FPlayTradeXGenerateWalletDynamicDelegate Callback)
{
    GenerateWallet(
        FPlayTradeXGenerateWalletDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXGenerateWalletResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    )
    );
}


// ============================================================
// Transaction Event Subscription
// ============================================================

FPlayTradeXTransactionEventListenerId
UPlayTradeXSubsystem::
SubscribeTransactionEvents(
    FPlayTradeXTransactionEventDelegate Callback)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    return static_cast<
        FPlayTradeXTransactionEventListenerId
    >(
        PlayTradeX::SDK::
        SubscribeTransactionEvents(
            [
                WeakThis,
                Callback = MoveTemp(Callback)
            ](
                const PlayTradeX::SDK::
                TransactionEvent& NativeEvent
                ) mutable
            {
                FPlayTradeXTransactionEvent Event =
                    ConvertTransactionEvent(
                        NativeEvent
                    );

                AsyncTask(
                    ENamedThreads::GameThread,
                    [
                        WeakThis,
                        Callback,
                        Event = MoveTemp(Event)
                    ]() mutable
                    {
                        if (!WeakThis.IsValid())
                        {
                            return;
                        }

                        Callback.ExecuteIfBound(
                            Event
                        );
                    }
                );
            }
        )
        );
}


bool
UPlayTradeXSubsystem::
UnsubscribeTransactionEvents(
    const FPlayTradeXTransactionEventListenerId ListenerId)
{
    if (ListenerId == 0)
    {
        return false;
    }

    return PlayTradeX::SDK::
        UnsubscribeTransactionEvents(
            static_cast<
            PlayTradeX::SDK::
            TransactionEventListenerId
            >(
                ListenerId
                )
        );
}


int64
UPlayTradeXSubsystem::
K2_SubscribeTransactionEvents(
    FPlayTradeXTransactionEventDynamicDelegate Callback)
{
    const FPlayTradeXTransactionEventListenerId
        ListenerId =
        SubscribeTransactionEvents(
            FPlayTradeXTransactionEventDelegate::
            CreateLambda(
                [
                    Callback = MoveTemp(Callback)
                ](
                    const FPlayTradeXTransactionEvent& Event
                    ) mutable
                {
                    Callback.ExecuteIfBound(
                        Event
                    );
                }
                        )
        );

    if (ListenerId >
        static_cast<uint64>(
            MAX_int64))
    {
        UnsubscribeTransactionEvents(
            ListenerId
        );

        return 0;
    }

    return static_cast<int64>(
        ListenerId
        );
}


bool
UPlayTradeXSubsystem::
K2_UnsubscribeTransactionEvents(
    const int64 ListenerId)
{
    if (ListenerId <= 0)
    {
        return false;
    }

    return UnsubscribeTransactionEvents(
        static_cast<uint64>(
            ListenerId
            )
    );
}


// ============================================================
// Transaction Consent
// ============================================================

void
UPlayTradeXSubsystem::
SetTransactionConsentCallback(
    FPlayTradeXTransactionConsentDelegate Callback)
{
    if (!Callback.IsBound())
    {
        PlayTradeX::SDK::
            SetTransactionConsentCallback({});

        return;
    }

    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::
        SetTransactionConsentCallback(
            [
                WeakThis,
                Callback = MoveTemp(Callback)
            ](
                const PlayTradeX::SDK::
                PreparedTransaction& NativeTransaction
                ) mutable
            {
                FPlayTradeXPreparedTransaction Transaction =
                    ConvertPreparedTransaction(
                        NativeTransaction
                    );

                AsyncTask(
                    ENamedThreads::GameThread,
                    [
                        WeakThis,
                        Callback,
                        Transaction = MoveTemp(Transaction)
                    ]() mutable
                    {
                        if (!WeakThis.IsValid())
                        {
                            return;
                        }

                        Callback.ExecuteIfBound(
                            Transaction
                        );
                    }
                );
            }
        );
}


void
UPlayTradeXSubsystem::
K2_SetTransactionConsentCallback(
    FPlayTradeXTransactionConsentDynamicDelegate Callback)
{
    if (!Callback.IsBound())
    {
        SetTransactionConsentCallback(
            FPlayTradeXTransactionConsentDelegate()
        );

        return;
    }

    SetTransactionConsentCallback(
        FPlayTradeXTransactionConsentDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXPreparedTransaction& Transaction
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Transaction
                );
            }
                    )
    );
}


void
UPlayTradeXSubsystem::
K2_ClearTransactionConsentCallback()
{
    SetTransactionConsentCallback(
        FPlayTradeXTransactionConsentDelegate()
    );
}


bool
UPlayTradeXSubsystem::
ApproveTransaction(
    const FString& TransactionId)
{
    return PlayTradeX::SDK::
        ApproveTransaction(
            ToStdString(
                TransactionId
            )
        );
}


bool
UPlayTradeXSubsystem::
DenyTransaction(
    const FString& TransactionId)
{
    return PlayTradeX::SDK::
        DenyTransaction(
            ToStdString(
                TransactionId
            )
        );
}


bool
UPlayTradeXSubsystem::
K2_ApproveTransaction(
    const FString& TransactionId)
{
    return ApproveTransaction(
        TransactionId
    );
}


bool
UPlayTradeXSubsystem::
K2_DenyTransaction(
    const FString& TransactionId)
{
    return DenyTransaction(
        TransactionId
    );
}


// ============================================================
// Native Balance
// Identity Wallet
// ============================================================

void UPlayTradeXSubsystem::GetNativeBalance(
    const FString& NetworkId,
    FPlayTradeXNativeBalanceDelegate Callback,
    const bool bSequential)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::GetNativeBalance(
        ToStdString(NetworkId),
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            NativeBalanceResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXNativeBalanceResponse Result =
                ConvertNativeBalanceResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        },
        bSequential
    );
}


// ============================================================
// Native Balance
// Address
// ============================================================

void UPlayTradeXSubsystem::GetNativeBalance(
    const FString& NetworkId,
    const FString& Address,
    FPlayTradeXNativeBalanceDelegate Callback,
    const bool bSequential)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::GetNativeBalance(
        ToStdString(NetworkId),
        ToStdString(Address),
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            NativeBalanceResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXNativeBalanceResponse Result =
                ConvertNativeBalanceResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        },
        bSequential
    );
}


void
UPlayTradeXSubsystem::
K2_GetNativeBalance(
    const FString& NetworkId,
    FPlayTradeXNativeBalanceDynamicDelegate Callback,
    const bool bSequential)
{
    GetNativeBalance(
        NetworkId,
        FPlayTradeXNativeBalanceDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXNativeBalanceResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    ),
        bSequential
    );
}


void
UPlayTradeXSubsystem::
K2_GetNativeBalanceForAddress(
    const FString& NetworkId,
    const FString& Address,
    FPlayTradeXNativeBalanceDynamicDelegate Callback,
    const bool bSequential)
{
    GetNativeBalance(
        NetworkId,
        Address,
        FPlayTradeXNativeBalanceDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXNativeBalanceResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    ),
        bSequential
    );
}


// ============================================================
// Send ETH
// ============================================================

void UPlayTradeXSubsystem::SendEth(
    const FPlayTradeXExecutionContext& Context,
    const FString& To,
    const FString& Amount,
    FPlayTradeXTransactionDelegate Callback,
    const bool bSequential)
{
    const PlayTradeX::SDK::ExecutionContext
        NativeContext =
        ConvertExecutionContext(
            Context
        );

    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::SendEth(
        NativeContext,
        ToStdString(To),
        ToStdString(Amount),
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            TransactionResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXTransactionResponse Result =
                ConvertTransactionResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        },
        bSequential
    );
}


void UPlayTradeXSubsystem::K2_SendEth(
    const FPlayTradeXExecutionContext& Context,
    const FString& To,
    const FString& Amount,
    FPlayTradeXTransactionDynamicDelegate Callback,
    const bool bSequential)
{
    SendEth(
        Context,
        To,
        Amount,
        FPlayTradeXTransactionDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXTransactionResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    ),
        bSequential
    );
}


// ============================================================
// Write
// ============================================================

void UPlayTradeXSubsystem::Write(
    const FPlayTradeXExecutionContext& Context,
    const FString& ContractAddress,
    const FString& Abi,
    const FString& FunctionName,
    const FString& Params,
    FPlayTradeXTransactionDelegate Callback,
    const FString& Value,
    const bool bSequential)
{
    const PlayTradeX::SDK::ExecutionContext
        NativeContext =
        ConvertExecutionContext(
            Context
        );

    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::Write(
        NativeContext,
        ToStdString(ContractAddress),
        ToStdString(Abi),
        ToStdString(FunctionName),
        ToStdString(Params),
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            TransactionResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXTransactionResponse Result =
                ConvertTransactionResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        },
        ToStdString(Value),
        bSequential
    );
}


void UPlayTradeXSubsystem::K2_Write(
    const FPlayTradeXExecutionContext& Context,
    const FString& ContractAddress,
    const FString& Abi,
    const FString& FunctionName,
    const FString& Params,
    FPlayTradeXTransactionDynamicDelegate Callback,
    const FString& Value,
    const bool bSequential)
{
    Write(
        Context,
        ContractAddress,
        Abi,
        FunctionName,
        Params,
        FPlayTradeXTransactionDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXTransactionResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    ),
        Value,
        bSequential
    );
}


// ============================================================
// Read
// ============================================================

void UPlayTradeXSubsystem::Read(
    const FString& NetworkId,
    const FString& ContractAddress,
    const FString& Abi,
    const FString& FunctionName,
    const FString& Params,
    FPlayTradeXContractReadDelegate Callback,
    const bool bSequential)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::Read(
        ToStdString(NetworkId),
        ToStdString(ContractAddress),
        ToStdString(Abi),
        ToStdString(FunctionName),
        ToStdString(Params),
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            ContractReadResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXContractReadResponse Result =
                ConvertContractReadResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        },
        bSequential
    );
}


void UPlayTradeXSubsystem::K2_Read(
    const FString& NetworkId,
    const FString& ContractAddress,
    const FString& Abi,
    const FString& FunctionName,
    const FString& Params,
    FPlayTradeXContractReadDynamicDelegate Callback,
    const bool bSequential)
{
    Read(
        NetworkId,
        ContractAddress,
        Abi,
        FunctionName,
        Params,
        FPlayTradeXContractReadDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXContractReadResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    ),
        bSequential
    );
}


// ============================================================
// Human Readable ABI
// ============================================================

void UPlayTradeXSubsystem::HumanReadableAbi(
    const FString& Abi,
    const bool bMinimal,
    FPlayTradeXHumanReadableAbiDelegate Callback)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    PlayTradeX::SDK::HumanReadableAbi(
        ToStdString(Abi),
        bMinimal,
        [
            WeakThis,
            Callback = MoveTemp(Callback)
        ](
            const PlayTradeX::SDK::
            HumanReadableAbiResponse& NativeResponse
            ) mutable
        {
            FPlayTradeXHumanReadableAbiResponse Result =
                ConvertHumanReadableAbiResponse(
                    NativeResponse
                );

            AsyncTask(
                ENamedThreads::GameThread,
                [
                    WeakThis,
                    Callback = MoveTemp(Callback),
                    Result = MoveTemp(Result)
                ]() mutable
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }

                    Callback.ExecuteIfBound(
                        Result
                    );
                }
            );
        }
    );
}


void
UPlayTradeXSubsystem::
K2_HumanReadableAbi(
    const FString& Abi,
    const bool bMinimal,
    FPlayTradeXHumanReadableAbiDynamicDelegate Callback)
{
    HumanReadableAbi(
        Abi,
        bMinimal,
        FPlayTradeXHumanReadableAbiDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXHumanReadableAbiResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(
                    Result
                );
            }
                    )
    );
}


// ============================================================
// Export Wallet
// ============================================================

void UPlayTradeXSubsystem::ExportWallet(
    const FString& Password,
    FPlayTradeXWalletExportDelegate Callback,
    const bool bSequential)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    const bool bOpened =
        FPlayTradeXFilePicker::SaveWalletFile(
            [
                WeakThis,
                Password,
                Callback = MoveTemp(Callback),
                bSequential
            ](
                const FString& Location
                ) mutable
            {
                if (!WeakThis.IsValid())
                {
                    return;
                }

                // User cancelled the picker.
                if (Location.IsEmpty())
                {
                    FPlayTradeXWalletExportResponse Result;

                    Result.Success = false;
                    Result.ErrorMessage =
                        TEXT("Wallet export was cancelled.");

                    Callback.ExecuteIfBound(Result);

                    return;
                }

                PlayTradeX::SDK::ExportWallet(
                    ToStdString(Password),
                    ToStdString(Location),
                    [
                        WeakThis,
                        Callback = MoveTemp(Callback)
                    ](
                        const PlayTradeX::SDK::
                        WalletExportResponse& NativeResponse
                        ) mutable
                    {
                        FPlayTradeXWalletExportResponse Result =
                            ConvertWalletExportResponse(
                                NativeResponse
                            );

                        AsyncTask(
                            ENamedThreads::GameThread,
                            [
                                WeakThis,
                                Callback = MoveTemp(Callback),
                                Result = MoveTemp(Result)
                            ]() mutable
                            {
                                if (!WeakThis.IsValid())
                                {
                                    return;
                                }

                                Callback.ExecuteIfBound(
                                    Result
                                );
                            }
                        );
                    },
                    bSequential
                );
            }
        );

    if (!bOpened)
    {
        FPlayTradeXWalletExportResponse Result;

        Result.Success = false;
        Result.ErrorMessage =
            TEXT("Could not open wallet export picker.");

        Callback.ExecuteIfBound(Result);
    }
}

void UPlayTradeXSubsystem::K2_ExportWallet(
    const FString& Password,
    FPlayTradeXWalletExportDynamicDelegate Callback,
    const bool bSequential)
{
    ExportWallet(
        Password,
        FPlayTradeXWalletExportDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXWalletExportResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(Result);
            }
                    ),
        bSequential
    );
}


// ============================================================
// Import Wallet
// ============================================================

void UPlayTradeXSubsystem::ImportWallet(
    const FString& Password,
    FPlayTradeXWalletImportDelegate Callback)
{
    TWeakObjectPtr<UPlayTradeXSubsystem>
        WeakThis(this);

    const bool bOpened =
        FPlayTradeXFilePicker::OpenWalletFile(
            [
                WeakThis,
                Password,
                Callback = MoveTemp(Callback)
            ](
                const FString& Location
                ) mutable
            {
                if (!WeakThis.IsValid())
                {
                    return;
                }

                // User cancelled the picker.
                if (Location.IsEmpty())
                {
                    FPlayTradeXWalletImportResponse Result;

                    Result.Success = false;
                    Result.ErrorMessage =
                        TEXT("Wallet import was cancelled.");

                    Callback.ExecuteIfBound(Result);

                    return;
                }

                PlayTradeX::SDK::ImportWallet(
                    ToStdString(Password),
                    ToStdString(Location),
                    [
                        WeakThis,
                        Callback = MoveTemp(Callback)
                    ](
                        const PlayTradeX::SDK::
                        WalletImportResponse& NativeResponse
                        ) mutable
                    {
                        FPlayTradeXWalletImportResponse Result =
                            ConvertWalletImportResponse(
                                NativeResponse
                            );

                        AsyncTask(
                            ENamedThreads::GameThread,
                            [
                                WeakThis,
                                Callback = MoveTemp(Callback),
                                Result = MoveTemp(Result)
                            ]() mutable
                            {
                                if (!WeakThis.IsValid())
                                {
                                    return;
                                }

                                Callback.ExecuteIfBound(
                                    Result
                                );
                            }
                        );
                    }
                );
            }
        );

    if (!bOpened)
    {
        FPlayTradeXWalletImportResponse Result;

        Result.Success = false;
        Result.ErrorMessage =
            TEXT("Could not open wallet import picker.");

        Callback.ExecuteIfBound(Result);
    }
}

void UPlayTradeXSubsystem::K2_ImportWallet(
    const FString& Password,
    FPlayTradeXWalletImportDynamicDelegate Callback)
{
    ImportWallet(
        Password,
        FPlayTradeXWalletImportDelegate::
        CreateLambda(
            [
                Callback = MoveTemp(Callback)
            ](
                const FPlayTradeXWalletImportResponse& Result
                ) mutable
            {
                Callback.ExecuteIfBound(Result);
            }
                    )
    );
}