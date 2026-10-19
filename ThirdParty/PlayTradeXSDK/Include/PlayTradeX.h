#pragma once

/**
 * @file PlayTradeX.h
 * @brief Public C++ API for the PlayTradeX SDK.
 *
 * Provides SDK initialization, multi-network blockchain access, standalone
 * wallet generation, blockchain queries, smart contract interaction,
 * transaction consent, native currency transfers, and identity-wallet
 * backup operations.
 *
 * PlayTradeX supports multiple configured EVM-compatible blockchain networks.
 * Network-specific operations select a network either through a network ID or
 * through an ExecutionContext, depending on whether the operation requires
 * transaction signing.
 *
 * Signed blockchain operations use ExecutionContext to select both the target
 * network and the transaction signer. The signer may be the SDK-managed
 * PlayTradeX identity wallet or an application-supplied external wallet.
 *
 * Applications should initialize the SDK using Initialize() before performing
 * runtime blockchain operations and should call Shutdown() before application
 * termination.
 *
 * @author Ektishaf Ahwaz
 *
 * @copyright Copyright (c) 2026 PlayTradeX.
 * All rights reserved.
 */

#include "PlayTradeXTypes.h"

#include <string>
#include <vector>

#if defined(_WIN32)

#if defined(PLAYTRADEX_SHARED)

#if defined(PLAYTRADEX_BUILD)
#define PLAYTRADEX_SDK_API __declspec(dllexport)
#else
#define PLAYTRADEX_SDK_API __declspec(dllimport)
#endif

#else

#define PLAYTRADEX_SDK_API

#endif

#else

#define PLAYTRADEX_SDK_API

#endif

namespace PlayTradeX::SDK
{
    /**
     * @brief Initializes the PlayTradeX SDK.
     *
     * Initializes the SDK runtime, persistent storage, blockchain network
     * registry, identity wallet state, authentication session, and required
     * internal services.
     *
     * Multiple blockchain networks may be supplied through the networks
     * collection. Each network is identified by its NetworkConfig::id and may
     * provide one or more JSON-RPC endpoints.
     *
     * The storage path must refer to a writable persistent directory that the
     * SDK can use for its local storage requirements.
     *
     * Initialization completes asynchronously. The supplied callback is invoked
     * when initialization either succeeds or fails.
     *
     * @param storagePath Writable persistent directory used by the SDK for
     * local storage.
     * @param networks Blockchain network configurations that should be
     * registered with the SDK.
     * @param callback Callback invoked when initialization completes.
     *
     * @note Runtime blockchain operations should only be performed after
     * initialization has completed successfully.
     *
     * @see NetworkConfig
     * @see Shutdown()
     * @see IsInitialized()
     */
    PLAYTRADEX_SDK_API void Initialize(const std::string& storagePath, const std::vector<NetworkConfig>& networks, InitializeCallback callback);

    /**
     * @brief Shuts down the PlayTradeX SDK.
     *
     * Stops SDK worker processing, clears active session credentials and
     * network configuration, removes pending transaction consent state, and
     * releases runtime state owned by the SDK.
     *
     * New SDK operations should not be submitted after shutdown has begun.
     *
     * @note Already executing or internally queued work may follow the SDK's
     * normal shutdown/drain behavior before shutdown completes.
     *
     * @see Initialize()
     * @see IsInitialized()
     */
    PLAYTRADEX_SDK_API void Shutdown();

    /**
     * @brief Determines whether the PlayTradeX SDK is initialized.
     *
     * @return true if the SDK is currently initialized; otherwise false.
     *
     * @see Initialize()
     * @see Shutdown()
     */
    PLAYTRADEX_SDK_API bool IsInitialized();

    /**
     * @brief Generates a new standalone EVM wallet.
     *
     * Creates a new wallet and returns its address, public key, and private key
     * through the supplied callback.
     *
     * The generated wallet is independent of the SDK-managed identity wallet.
     * Generating a wallet does not replace the identity wallet used by the
     * PlayTradeX initialization and handshake flow.
     *
     * The SDK does not automatically persist the generated wallet as part of
     * the PlayTradeX identity. The calling application is responsible for
     * deciding how the returned wallet is used or stored.
     *
     * @param callback Callback invoked when wallet generation completes.
     * processing; otherwise normal concurrent processing is permitted.
     *
     * @warning The generated response contains the wallet private key.
     * Applications must treat this value as sensitive wallet credentials.
     */
    PLAYTRADEX_SDK_API void GenerateWallet(GenerateWalletCallback callback);

    /**
     * @brief Subscribes to mined transaction events.
     *
     * The callback is invoked when a transaction submitted
     * through SendEth() or Write() reaches a terminal mined
     * state.
     *
     * A transaction may be reported as:
     * - Confirmed: mined successfully.
     * - Reverted: mined but execution reverted.
     *
     * The event contains the transaction receipt.
     *
     * @param callback Callback invoked for mined transactions.
     *
     * @return A non-zero listener ID on success.
     *         Returns 0 if the callback is invalid.
     */
    PLAYTRADEX_SDK_API TransactionEventListenerId SubscribeTransactionEvents(TransactionEventCallback callback);

    /**
     * @brief Removes a previously registered transaction
     * event listener.
     *
     * @param listenerId Listener ID returned by
     * SubscribeTransactionEvents().
     *
     * @return true if the listener was removed.
     *         false if the listener ID was not registered.
     */
    PLAYTRADEX_SDK_API bool UnsubscribeTransactionEvents(TransactionEventListenerId listenerId);

    /**
     * @brief Registers the callback used to request user consent for transactions.
     *
     * State-changing blockchain operations such as SendEth() and Write()
     * require the SDK to prepare a transaction before it can be signed and
     * submitted.
     *
     * The callback receives a PreparedTransaction containing the information
     * available for review, including the unique transaction ID, network,
     * chain ID, resolved sender address, transaction target, value, gas and
     * fee information, and simulation status.
     *
     * The application should respond by calling either ApproveTransaction()
     * or DenyTransaction() with that transaction ID.
     *
     * Passing an empty callback unregisters the currently registered callback.
     *
     * @param callback Callback invoked when a prepared transaction requires
     * consent.
     *
     * @note Applications should normally register the callback before initiating
     * state-changing blockchain operations.
     *
     * @see PreparedTransaction
     * @see ApproveTransaction()
     * @see DenyTransaction()
     */
    PLAYTRADEX_SDK_API void SetTransactionConsentCallback(TransactionConsentCallback callback);

    /**
     * @brief Denies a previously prepared transaction.
     *
     * Removes the corresponding pending transaction from the SDK. A denied
     * transaction is not signed or submitted to the blockchain.
     *
     * @param transactionId Transaction ID received through the registered
     * transaction consent callback.
     *
     * @return true if the pending transaction was found and successfully
     * denied; otherwise false.
     *
     * @see SetTransactionConsentCallback()
     * @see ApproveTransaction()
     */
    PLAYTRADEX_SDK_API bool DenyTransaction(const std::string& transactionId);

    /**
     * @brief Approves a previously prepared transaction.
     *
     * Allows the SDK to continue with signing and submission of the exact
     * transaction associated with the supplied transaction ID.
     *
     * The signer resolved from the original ExecutionContext remains associated
     * with the pending transaction during the consent flow. Approval therefore
     * continues using the signer selected when the transaction was prepared.
     *
     * @param transactionId Transaction ID received through the registered
     * transaction consent callback.
     *
     * @return true if the pending transaction was found and accepted for
     * approval; otherwise false.
     *
     * @note Approval can fail if the transaction is no longer pending,
     * has expired, cannot safely be submitted, or the transaction ID is invalid.
     *
     * @see SetTransactionConsentCallback()
     * @see DenyTransaction()
     */
    PLAYTRADEX_SDK_API bool ApproveTransaction(const std::string& transactionId);

    /**
     * @brief Retrieves the native blockchain balance of the SDK identity wallet.
     *
     * Queries the selected configured blockchain network for the native currency
     * balance of the PlayTradeX identity wallet managed by the SDK.
     *
     * @param networkId Identifier of the configured blockchain network to query.
     * @param callback Callback invoked when the request completes.
     * @param sequential If true, the request uses sequential request execution;
     * otherwise normal concurrent execution is permitted.
     *
     * @note The SDK must be successfully initialized before calling this
     * function.
     *
     * @see GetNativeBalance(const std::string&, const std::string&,
     * NativeBalanceCallback, bool)
     */
    PLAYTRADEX_SDK_API void GetNativeBalance(const std::string& networkId, NativeBalanceCallback callback, bool sequential = false);

    /**
     * @brief Retrieves the native blockchain balance of a specified address.
     *
     * Queries the selected configured blockchain network for the native currency
     * balance associated with the supplied EVM address.
     *
     * Because this is a read-only blockchain query, possession of the private
     * key corresponding to the supplied address is not required.
     *
     * This overload is useful for querying application-managed wallets,
     * generated test wallets, externally created wallets, or any other valid
     * EVM address.
     *
     * @param networkId Identifier of the configured blockchain network to query.
     * @param address EVM address whose native balance should be queried.
     * @param callback Callback invoked when the request completes.
     * @param sequential If true, the request uses sequential request execution;
     * otherwise normal concurrent execution is permitted.
     *
     * @note The SDK must be successfully initialized before calling this
     * function.
     *
     * @see GetNativeBalance(const std::string&, NativeBalanceCallback, bool)
     */
    PLAYTRADEX_SDK_API void GetNativeBalance(const std::string& networkId, const std::string& address, NativeBalanceCallback callback, bool sequential = false);

    /**
     * @brief Sends native blockchain currency using the execution context.
     *
     * Prepares a native currency transfer on the network selected by
     * ExecutionContext::networkId.
     *
     * The transaction signer is resolved from ExecutionContext::walletSource.
     * The SDK-managed identity wallet is used when WalletSource::Identity is
     * selected. When WalletSource::External is selected, the external private
     * key supplied by the execution context is used instead.
     *
     * The transaction is presented through the registered transaction consent
     * callback before signing and submission.
     *
     * @param context Execution context selecting the blockchain network and
     * transaction signer.
     * @param to Recipient EVM account address.
     * @param amount Native currency amount in the SDK-supported amount format.
     * @param callback Callback invoked when the transaction operation completes.
     * @param sequential If true, internal request processing uses sequential
     * execution where supported; otherwise concurrent execution is permitted.
     *
     * @note The SDK must be successfully initialized before calling this
     * function.
     *
     * @warning This operation can submit a real blockchain transaction and
     * consume native currency for both the transferred value and network fees.
     *
     * @see ExecutionContext
     * @see WalletSource
     * @see SetTransactionConsentCallback()
     * @see ApproveTransaction()
     * @see DenyTransaction()
     */
    PLAYTRADEX_SDK_API void SendEth(const ExecutionContext& context, const std::string& to, const std::string& amount, TransactionCallback callback, bool sequential = false);

    /**
     * @brief Begins a state-changing smart contract operation using the supplied
     * execution context.
     *
     * Encodes the requested contract function call and prepares a blockchain
     * transaction on the network selected by ExecutionContext::networkId.
     *
     * The transaction signer is resolved from ExecutionContext::walletSource.
     * The SDK-managed identity wallet is used when WalletSource::Identity is
     * selected. When WalletSource::External is selected, the external private
     * key supplied by the execution context is used instead.
     *
     * Before the transaction is signed or submitted, the SDK invokes the
     * registered transaction consent callback with the prepared transaction
     * details.
     *
     * The application must explicitly approve or deny the pending transaction.
     *
     * @param context Execution context selecting the blockchain network and
     * transaction signer.
     * @param contractAddress Target smart contract address.
     * @param abi Contract ABI in the format supported by the SDK.
     * @param functionName Name of the contract function to invoke.
     * @param params Function parameters encoded using the SDK-supported JSON
     * parameter format.
     * @param callback Callback invoked when the transaction operation completes.
     * @param value Native blockchain value attached to the transaction.
     * Use "0" when no native value should be transferred.
     * @param sequential If true, internal request processing uses sequential
     * execution where supported; otherwise concurrent execution is permitted.
     *
     * @note The SDK must be successfully initialized before calling this
     * function.
     *
     * @warning When WalletSource::External is used, the execution context
     * contains sensitive private-key material that should not be logged or
     * unnecessarily retained by the application.
     *
     * @warning This operation can modify blockchain state and consume native
     * currency for network fees and, when value is non-zero, transferred value.
     *
     * @see ExecutionContext
     * @see WalletSource
     * @see SetTransactionConsentCallback()
     * @see ApproveTransaction()
     * @see DenyTransaction()
     */
    PLAYTRADEX_SDK_API void Write(const ExecutionContext& context, const std::string& contractAddress, const std::string& abi, const std::string& functionName, const std::string& params, TransactionCallback callback, const std::string& value = "0", bool sequential = false);

    /**
     * @brief Executes a read-only smart contract call.
     *
     * Executes a contract call against the selected configured blockchain
     * network without creating or signing a blockchain transaction.
     *
     * Read operations do not require an ExecutionContext because they do not
     * require transaction signing. The target blockchain is selected directly
     * using networkId.
     *
     * Read operations do not require transaction consent and do not directly
     * modify blockchain state.
     *
     * @param networkId Identifier of the configured blockchain network to query.
     * @param contractAddress Target smart contract address.
     * @param abi Contract ABI in the format supported by the SDK.
     * @param functionName Name of the contract function to invoke.
     * @param params Function parameters encoded using the SDK-supported JSON
     * parameter format.
     * @param callback Callback invoked when the read operation completes.
     * @param sequential If true, the request uses sequential request execution;
     * otherwise normal concurrent execution is permitted.
     *
     * @note The SDK must be successfully initialized before calling this
     * function.
     */
    PLAYTRADEX_SDK_API void Read(const std::string& networkId, const std::string& contractAddress, const std::string& abi, const std::string& functionName, const std::string& params, ContractReadCallback callback, bool sequential = false);

    /**
     * @brief Converts a contract ABI into a human-readable representation.
     *
     * Converts a standard smart contract ABI JSON representation into the
     * human-readable ABI format supported by PlayTradeX contract operations.
     *
     * This function is intended primarily as a developer/editor utility rather
     * than a normal runtime gameplay operation.
     *
     * @param abi Contract ABI JSON.
     * @param minimal If true, requests a minimal human-readable representation.
     * @param callback Callback invoked when ABI conversion completes.
     * otherwise normal concurrent execution is permitted.
     *
     * @note This utility does not require transaction consent.
     */
    PLAYTRADEX_SDK_API void HumanReadableAbi(const std::string& abi, bool minimal, HumanReadableAbiCallback callback);

    /**
     * @brief Exports the locally stored PlayTradeX identity wallet to an
     * encrypted backup.
     *
     * Creates an encrypted PlayTradeX wallet backup file at the specified
     * destination.
     *
     * This operation applies to the SDK-managed identity wallet. Standalone
     * wallets returned by GenerateWallet() and external wallets supplied through
     * ExecutionContext are application-managed and are not exported by this
     * function.
     *
     * The identity wallet private key is encrypted using key material derived
     * from the supplied password and is not written to the backup in plaintext.
     *
     * @param password Password used to protect the wallet backup.
     * @param outputPath Destination path for the PlayTradeX wallet backup file.
     * @param callback Callback invoked when the export operation completes.
     * @param sequential If true, the operation uses sequential request
     *
     * @note The SDK must be successfully initialized before exporting the
     * identity wallet.
     *
     * @warning The backup password is required to decrypt and restore the
     * exported wallet. Applications should not log, persist, or expose the
     * password unnecessarily.
     *
     * @see ImportWallet()
     */
    PLAYTRADEX_SDK_API void ExportWallet(const std::string& password, const std::string& outputPath, SDK::WalletExportCallback callback, bool sequential = false);

    /**
     * @brief Imports an encrypted PlayTradeX identity wallet backup.
     *
     * Decrypts the supplied PlayTradeX wallet backup using the provided
     * password, validates the recovered private key against the wallet address
     * stored in the backup, and saves the validated wallet into the SDK's local
     * secure storage.
     *
     * This operation applies to the SDK-managed identity wallet. It does not
     * import or register an application-managed external wallet.
     *
     * A successful import replaces the identity wallet data that will be used
     * during the next SDK initialization.
     *
     * @param password Password that was used to protect the wallet backup.
     * @param inputPath Path to the encrypted PlayTradeX wallet backup file.
     * @param callback Callback invoked when the import operation completes.
     *
     * @warning After a successful import, the application should shut down and
     * restart/reinitialize the SDK before performing further identity-wallet
     * operations.
     *
     * @warning Importing a wallet changes the identity wallet that will be
     * loaded by the SDK on the next initialization.
     *
     * @see ExportWallet()
     * @see Shutdown()
     * @see Initialize()
     */
    PLAYTRADEX_SDK_API void ImportWallet(const std::string& password, const std::string& inputPath, SDK::WalletImportCallback callback);
} // namespace PlayTradeX::SDK