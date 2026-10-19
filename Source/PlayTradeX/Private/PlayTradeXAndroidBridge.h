#pragma once

#include "CoreMinimal.h"

class FPlayTradeXAndroidBridge
{
public:

    static bool Initialize();

    using FPickerCallback =
        TFunction<void(const FString& Uri)>;

    static bool OpenImportPicker(
        FPickerCallback Callback
    );

    static bool OpenExportPicker(
        FPickerCallback Callback
    );

    static void HandleImportPickerResult(
        const FString& Uri
    );

    static void HandleExportPickerResult(
        const FString& Uri
    );
};