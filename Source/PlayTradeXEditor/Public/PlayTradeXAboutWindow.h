#pragma once

#include "CoreMinimal.h"

class SWindow;

/**
 * PlayTradeX About window.
 *
 * Displays release information, supported platforms,
 * SDK capabilities and PlayTradeX resources.
 */
class FPlayTradeXAboutWindow
{
public:

    /**
     * Opens the About PlayTradeX window.
     *
     * If the window is already open, the existing window
     * is brought to the front.
     */
    static void Open();

private:

    // ============================================================
    // Window
    // ============================================================

    static TSharedRef<SWindow> CreateWindow();


    // ============================================================
    // Information
    // ============================================================

    static FString GetPluginVersion();

    static FString GetReleaseChannel(
        const FString& Version
    );

    static FString GetInstallationSource();

    static FString GetInstalledRevision();

    static FString GetUnrealEngineVersion();

    static FString GetReleaseDisclaimer(
        const FString& ReleaseChannel
    );


    // ============================================================
    // External Resources
    // ============================================================

    static void OpenUrl(
        const FString& Url
    );

    static void OpenDocumentation();

    static void OpenRepository();

    static void OpenIssues();

    static void OpenUpdater();


    // ============================================================
    // State
    // ============================================================

    static TWeakPtr<SWindow> AboutWindow;
};