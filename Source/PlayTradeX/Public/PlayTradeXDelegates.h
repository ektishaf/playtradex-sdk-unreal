#pragma once

#include "CoreMinimal.h"
#include "PlayTradeXUnrealTypes.h"

// ============================================================
// C++ Request Delegates
// ============================================================

DECLARE_DELEGATE_OneParam(
    FPlayTradeXInitializeDelegate,
    const FPlayTradeXInitializeResult&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXGenerateWalletDelegate,
    const FPlayTradeXGenerateWalletResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXNativeBalanceDelegate,
    const FPlayTradeXNativeBalanceResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXTransactionDelegate,
    const FPlayTradeXTransactionResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXHumanReadableAbiDelegate,
    const FPlayTradeXHumanReadableAbiResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXContractReadDelegate,
    const FPlayTradeXContractReadResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXWalletExportDelegate,
    const FPlayTradeXWalletExportResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXWalletImportDelegate,
    const FPlayTradeXWalletImportResponse&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXTransactionConsentDelegate,
    const FPlayTradeXPreparedTransaction&
);

DECLARE_DELEGATE_OneParam(
    FPlayTradeXTransactionEventDelegate,
    const FPlayTradeXTransactionEvent&
);


// ============================================================
// C++ Lifecycle Events
// ============================================================

DECLARE_MULTICAST_DELEGATE(
    FPlayTradeXReadyEvent
);

DECLARE_MULTICAST_DELEGATE(
    FPlayTradeXNotReadyEvent
);


// ============================================================
// Listener ID
// ============================================================

using FPlayTradeXTransactionEventListenerId = uint64;