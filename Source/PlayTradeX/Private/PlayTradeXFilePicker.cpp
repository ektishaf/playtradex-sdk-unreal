#include "PlayTradeXFilePicker.h"

#if PLATFORM_ANDROID
#include "PlayTradeXAndroidBridge.h"
#endif

#if PLATFORM_WINDOWS
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#endif


// ============================================================
// Open Wallet File
// ============================================================

bool FPlayTradeXFilePicker::OpenWalletFile(
    FPickerCallback Callback
)
{
    if (!Callback)
    {
        return false;
    }

#if PLATFORM_ANDROID

    return FPlayTradeXAndroidBridge::
        OpenImportPicker(
            MoveTemp(Callback)
        );

#elif PLATFORM_WINDOWS

    IDesktopPlatform* DesktopPlatform =
        FDesktopPlatformModule::Get();

    if (DesktopPlatform == nullptr)
    {
        return false;
    }

    TArray<FString> SelectedFiles;

    const bool bOpened =
        DesktopPlatform->OpenFileDialog(
            nullptr,
            TEXT("Import PlayTradeX Wallet"),
            FString(),
            FString(),
            TEXT("PlayTradeX Wallet (*.ptx)|*.ptx"),
            EFileDialogFlags::None,
            SelectedFiles
        );

    if (!bOpened ||
        SelectedFiles.IsEmpty())
    {
        Callback(FString());

        return true;
    }

    Callback(
        SelectedFiles[0]
    );

    return true;

#else

    return false;

#endif
}


// ============================================================
// Save Wallet File
// ============================================================

bool FPlayTradeXFilePicker::SaveWalletFile(
    FPickerCallback Callback
)
{
    if (!Callback)
    {
        return false;
    }

#if PLATFORM_ANDROID

    return FPlayTradeXAndroidBridge::
        OpenExportPicker(
            MoveTemp(Callback)
        );

#elif PLATFORM_WINDOWS

    IDesktopPlatform* DesktopPlatform =
        FDesktopPlatformModule::Get();

    if (DesktopPlatform == nullptr)
    {
        return false;
    }

    TArray<FString> SelectedFiles;

    const bool bOpened =
        DesktopPlatform->SaveFileDialog(
            nullptr,
            TEXT("Export PlayTradeX Wallet"),
            FString(),
            TEXT("playtradex-wallet.ptx"),
            TEXT("PlayTradeX Wallet (*.ptx)|*.ptx"),
            EFileDialogFlags::None,
            SelectedFiles
        );

    if (!bOpened ||
        SelectedFiles.IsEmpty())
    {
        Callback(FString());

        return true;
    }

    FString OutputPath =
        SelectedFiles[0];

    if (!OutputPath.EndsWith(
        TEXT(".ptx"),
        ESearchCase::IgnoreCase))
    {
        OutputPath +=
            TEXT(".ptx");
    }

    Callback(
        OutputPath
    );

    return true;

#else

    return false;

#endif
}