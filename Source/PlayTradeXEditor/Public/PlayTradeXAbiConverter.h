#pragma once

#include "CoreMinimal.h"

class SEditableTextBox;
class SMultiLineEditableTextBox;
class SWindow;

/**
 * Unreal Editor implementation of the PlayTradeX ABI Converter.
 *
 * Converts a JSON contract ABI through the native PlayTradeX SDK
 * and generates an Unreal C++ contract interface header.
 */
class FPlayTradeXAbiConverter
{
public:

    static void Open();

private:

    // ============================================================
    // Window
    // ============================================================

    static TSharedRef<SWindow> CreateWindow();

    static TSharedRef<SWidget> CreateHeader();

    static TSharedRef<SWidget> CreateAbiInput();

    static TSharedRef<SWidget> CreateSettings();

    static TSharedRef<SWidget> CreateActions();


    // ============================================================
    // Generation
    // ============================================================

    static FReply Generate();

    static FReply Cancel();

    static void HandleHumanReadableAbi(
        bool bSuccess,
        int32 ErrorCode,
        const FString& HumanReadableAbi,
        const FString& ErrorMessage
    );

    static bool GenerateContractHeader(
        const TArray<FString>& HumanReadableAbi
    );


    // ============================================================
    // Validation
    // ============================================================

    static bool ValidateInput(
        FString& OutError
    );

    static bool IsValidJson(
        const FString& Value
    );

    static bool IsValidEthereumAddress(
        const FString& Address
    );

    static bool IsValidCppIdentifier(
        const FString& Value
    );


    // ============================================================
    // ABI Processing
    // ============================================================

    static bool ParseHumanReadableAbi(
        const FString& Json,
        TArray<FString>& OutAbi
    );

    static bool TryCreateFunctionEntry(
        const FString& Raw,
        FString& OutIdentifier
    );

    static FString NormalizeTypeName(
        const FString& TypeName
    );

    static FString EscapeCppString(
        const FString& Value
    );

    static FString EscapeCppRawString(
        const FString& Value
    );


    // ============================================================
    // Paths
    // ============================================================

    static FString GetGeneratedContractsDirectory();

    static FString GetOutputPath();


    // ============================================================
    // Dialogs
    // ============================================================

    static void ShowError(
        const FString& Message
    );

    static void ShowSuccess(
        const FString& OutputPath
    );


    // ============================================================
    // State
    // ============================================================

    static TWeakPtr<SWindow> Window;

    static TSharedPtr<SMultiLineEditableTextBox> AbiTextBox;

    static TSharedPtr<SEditableTextBox> AddressTextBox;

    static TSharedPtr<SEditableTextBox> FileNameTextBox;

    static bool bSaveAbi;

    static bool bSaveHumanReadableAbi;

    static bool bIsGenerating;
};