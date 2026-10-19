#pragma once

#include "CoreMinimal.h"

/**
 * Platform-independent file picker used by PlayTradeX.
 *
 * Currently supported:
 * - Windows
 * - Android
 *
 * Future:
 * - macOS
 * - Linux
 * - iOS
 *
 * The returned FString represents:
 * - a filesystem path on desktop platforms
 * - a content URI on Android
 *
 * An empty string means the user cancelled the picker.
 */
class PLAYTRADEX_API FPlayTradeXFilePicker
{
public:

    using FPickerCallback =
        TFunction<void(const FString& Location)>;

    /**
     * Opens the platform file picker for selecting
     * an existing PlayTradeX wallet file.
     *
     * Returns false when the picker could not be launched.
     */
    static bool OpenWalletFile(
        FPickerCallback Callback
    );

    /**
     * Opens the platform file picker for selecting
     * the destination of an exported PlayTradeX wallet.
     *
     * Returns false when the picker could not be launched.
     */
    static bool SaveWalletFile(
        FPickerCallback Callback
    );
};