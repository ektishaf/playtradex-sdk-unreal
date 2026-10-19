#include "PlayTradeXAbiConverter.h"

#include "PlayTradeX.h"

#include "Async/Async.h"

#include "Dom/JsonValue.h"

#include "Framework/Application/SlateApplication.h"

#include "HAL/FileManager.h"

#include "Interfaces/IPluginManager.h"

#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "Styling/AppStyle.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSpacer.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/SWindow.h"

#include "Widgets/Text/STextBlock.h"


#define LOCTEXT_NAMESPACE "FPlayTradeXAbiConverter"


// ============================================================
// Static State
// ============================================================

TWeakPtr<SWindow>
FPlayTradeXAbiConverter::Window;

TSharedPtr<SMultiLineEditableTextBox>
FPlayTradeXAbiConverter::AbiTextBox;

TSharedPtr<SEditableTextBox>
FPlayTradeXAbiConverter::AddressTextBox;

TSharedPtr<SEditableTextBox>
FPlayTradeXAbiConverter::FileNameTextBox;

bool
FPlayTradeXAbiConverter::bSaveAbi = false;

bool
FPlayTradeXAbiConverter::bSaveHumanReadableAbi = false;

bool
FPlayTradeXAbiConverter::bIsGenerating = false;


// ============================================================
// Open
// ============================================================

void FPlayTradeXAbiConverter::Open()
{
    if (Window.IsValid())
    {
        const TSharedPtr<SWindow> ExistingWindow =
            Window.Pin();

        if (ExistingWindow.IsValid())
        {
            ExistingWindow->BringToFront(true);

            return;
        }
    }


    bSaveAbi =
        false;

    bSaveHumanReadableAbi =
        false;

    bIsGenerating =
        false;


    const TSharedRef<SWindow> NewWindow =
        CreateWindow();

    Window =
        NewWindow;


    FSlateApplication::Get().AddWindow(
        NewWindow
    );
}


// ============================================================
// Create Window
// ============================================================

TSharedRef<SWindow> FPlayTradeXAbiConverter::CreateWindow()
{
    const FString DefaultFileName =
        FString::Printf(
            TEXT("ContractName%d"),
            FMath::RandRange(
                1000,
                9999
            )
        );


    TSharedRef<SWindow> NewWindow =

        SNew(SWindow)

        .Title(
            LOCTEXT(
                "WindowTitle",
                "ABI Converter"
            )
        )

        .ClientSize(
            FVector2D(
                800.0f,
                800.0f
            )
        )

        .SizingRule(
            ESizingRule::UserSized
        )

        .SupportsMaximize(true)

        .SupportsMinimize(false);


    NewWindow->SetContent(

        SNew(SBorder)

        .BorderImage(
            FAppStyle::GetBrush(
                TEXT("Brushes.Panel")
            )
        )

        .Padding(
            16.0f
        )

        [

            SNew(SVerticalBox)


                // ====================================================
                // Header
                // ====================================================

                +SVerticalBox::Slot()

                .AutoHeight()

                [

                    CreateHeader()

                ]


                // ====================================================
                // ABI
                // ====================================================

                +SVerticalBox::Slot()

                .FillHeight(1.0f)

                .Padding(
                    0.0f,
                    16.0f,
                    0.0f,
                    0.0f
                )

                [

                    CreateAbiInput()

                ]


                // ====================================================
                // Settings
                // ====================================================

                +SVerticalBox::Slot()

                .AutoHeight()

                .Padding(
                    0.0f,
                    16.0f,
                    0.0f,
                    0.0f
                )

                [

                    CreateSettings()

                ]


                // ====================================================
                // Actions
                // ====================================================

                +SVerticalBox::Slot()

                .AutoHeight()

                .Padding(
                    0.0f,
                    20.0f,
                    0.0f,
                    0.0f
                )

                [

                    CreateActions()

                ]

        ]

    );


    AddressTextBox->SetText(
        FText::FromString(
            TEXT(
                "0x0000000000000000000000000000000000000000"
            )
        )
    );

    FileNameTextBox->SetText(
        FText::FromString(
            DefaultFileName
        )
    );


    return NewWindow;
}


// ============================================================
// Header
// ============================================================

TSharedRef<SWidget> FPlayTradeXAbiConverter::CreateHeader()
{
    return

        SNew(SVerticalBox)


        + SVerticalBox::Slot()

        .AutoHeight()

        .HAlign(HAlign_Center)

        [

            SNew(STextBlock)

                .Text(
                    LOCTEXT(
                        "Title",
                        "Contract ABI Converter"
                    )
                )

                .Font(
                    FCoreStyle::GetDefaultFontStyle(
                        TEXT("Bold"),
                        14
                    )
                )

        ]


    + SVerticalBox::Slot()

        .AutoHeight()

        .HAlign(HAlign_Center)

        .Padding(
            20.0f,
            6.0f,
            20.0f,
            0.0f
        )

        [

            SNew(STextBlock)

                .Text(
                    LOCTEXT(
                        "Description",
                        "Paste the complete JSON ABI of your smart contract below. "
                        "PlayTradeX will convert it to a human readable ABI and "
                        "generate a C++ contract interface."
                    )
                )

                .Font(
                    FCoreStyle::GetDefaultFontStyle(
                        TEXT("Regular"),
                        10
                    )
                )

                .Justification(
                    ETextJustify::Center
                )

                .AutoWrapText(true)

        ];
}


// ============================================================
// ABI Input
// ============================================================

TSharedRef<SWidget> FPlayTradeXAbiConverter::CreateAbiInput()
{
    return

        SNew(SVerticalBox)


        + SVerticalBox::Slot()

        .AutoHeight()

        [

            SNew(STextBlock)

                .Text(
                    LOCTEXT(
                        "ContractAbiLabel",
                        "Contract ABI"
                    )
                )

                .Font(
                    FAppStyle::GetFontStyle(
                        TEXT("PropertyWindow.BoldFont")
                    )
                )

        ]


    + SVerticalBox::Slot()

        .FillHeight(1.0f)

        .Padding(
            0.0f,
            6.0f,
            0.0f,
            0.0f
        )

        [

            SNew(SBorder)

                .BorderImage(
                    FAppStyle::GetBrush(
                        TEXT("ToolPanel.GroupBorder")
                    )
                )

                .Padding(
                    1.0f
                )

                [

                    SAssignNew(
                        AbiTextBox,
                        SMultiLineEditableTextBox
                    )

                        .Text(
                            FText::GetEmpty()
                        )

                        .AutoWrapText(true)

                        .AllowMultiLine(true)

                ]

        ]


    + SVerticalBox::Slot()

        .AutoHeight()

        .Padding(
            0.0f,
            6.0f,
            0.0f,
            0.0f
        )

        [

            SNew(STextBlock)

                .Text(
                    LOCTEXT(
                        "AbiHelp",
                        "Paste the complete contract JSON ABI into the field above."
                    )
                )

                .Font(
                    FAppStyle::GetFontStyle(
                        TEXT("SmallFont")
                    )
                )

        ];
}


// ============================================================
// Settings
// ============================================================

TSharedRef<SWidget> FPlayTradeXAbiConverter::CreateSettings()
{
    return

        SNew(SBorder)

        .BorderImage(
            FAppStyle::GetBrush(
                TEXT("ToolPanel.GroupBorder")
            )
        )

        .Padding(
            12.0f
        )

        [

            SNew(SVerticalBox)


                // ====================================================
                // Address
                // ====================================================

                +SVerticalBox::Slot()

                .AutoHeight()

                [

                    SNew(SHorizontalBox)


                        + SHorizontalBox::Slot()

                        .AutoWidth()

                        .VAlign(VAlign_Center)

                        [

                            SNew(SBox)

                                .WidthOverride(
                                    200.0f
                                )

                                [

                                    SNew(STextBlock)

                                        .Text(
                                            LOCTEXT(
                                                "AddressLabel",
                                                "Contract Address"
                                            )
                                        )

                                ]

                        ]


                    + SHorizontalBox::Slot()

                        .FillWidth(1.0f)

                        .VAlign(VAlign_Center)

                        [

                            SAssignNew(
                                AddressTextBox,
                                SEditableTextBox
                            )

                        ]

                ]


            // ====================================================
            // Filename
            // ====================================================

            +SVerticalBox::Slot()

                .AutoHeight()

                .Padding(
                    0.0f,
                    8.0f,
                    0.0f,
                    0.0f
                )

                [

                    SNew(SHorizontalBox)


                        + SHorizontalBox::Slot()

                        .AutoWidth()

                        .VAlign(VAlign_Center)

                        [

                            SNew(SBox)

                                .WidthOverride(
                                    200.0f
                                )

                                [

                                    SNew(STextBlock)

                                        .Text(
                                            LOCTEXT(
                                                "FilenameLabel",
                                                "Save Filename"
                                            )
                                        )

                                ]

                        ]


                    + SHorizontalBox::Slot()

                        .FillWidth(1.0f)

                        .VAlign(VAlign_Center)

                        [

                            SAssignNew(
                                FileNameTextBox,
                                SEditableTextBox
                            )

                        ]

                ]


            // ====================================================
            // Save ABI
            // ====================================================

            +SVerticalBox::Slot()

                .AutoHeight()

                .Padding(
                    0.0f,
                    8.0f,
                    0.0f,
                    0.0f
                )

                [

                    SNew(SHorizontalBox)


                        + SHorizontalBox::Slot()

                        .AutoWidth()

                        .VAlign(VAlign_Center)

                        [

                            SNew(SBox)

                                .WidthOverride(
                                    200.0f
                                )

                                [

                                    SNew(STextBlock)

                                        .Text(
                                            LOCTEXT(
                                                "SaveAbiLabel",
                                                "Save ABI"
                                            )
                                        )

                                ]

                        ]


                    + SHorizontalBox::Slot()

                        .AutoWidth()

                        .VAlign(VAlign_Center)

                        [

                            SNew(SCheckBox)

                                .IsChecked_Lambda(
                                    []()
                                    {
                                        return
                                            bSaveAbi
                                            ? ECheckBoxState::Checked
                                            : ECheckBoxState::Unchecked;
                                    }
                                )

                                .OnCheckStateChanged_Lambda(
                                    [](ECheckBoxState State)
                                    {
                                        bSaveAbi =
                                            State ==
                                            ECheckBoxState::Checked;
                                    }
                                )

                        ]

                ]


            // ====================================================
            // Save Human Readable ABI
            // ====================================================

            +SVerticalBox::Slot()

                .AutoHeight()

                .Padding(
                    0.0f,
                    6.0f,
                    0.0f,
                    0.0f
                )

                [

                    SNew(SHorizontalBox)


                        + SHorizontalBox::Slot()

                        .AutoWidth()

                        .VAlign(VAlign_Center)

                        [

                            SNew(SBox)

                                .WidthOverride(
                                    200.0f
                                )

                                [

                                    SNew(STextBlock)

                                        .Text(
                                            LOCTEXT(
                                                "SaveHumanAbiLabel",
                                                "Save Human Readable ABI"
                                            )
                                        )

                                ]

                        ]


                    + SHorizontalBox::Slot()

                        .AutoWidth()

                        .VAlign(VAlign_Center)

                        [

                            SNew(SCheckBox)

                                .IsChecked_Lambda(
                                    []()
                                    {
                                        return
                                            bSaveHumanReadableAbi
                                            ? ECheckBoxState::Checked
                                            : ECheckBoxState::Unchecked;
                                    }
                                )

                                .OnCheckStateChanged_Lambda(
                                    [](ECheckBoxState State)
                                    {
                                        bSaveHumanReadableAbi =
                                            State ==
                                            ECheckBoxState::Checked;
                                    }
                                )

                        ]

                ]

        ];
}


// ============================================================
// Actions
// ============================================================

TSharedRef<SWidget> FPlayTradeXAbiConverter::CreateActions()
{
    return

        SNew(SHorizontalBox)


        // ========================================================
        // Generate
        // ========================================================

        +SHorizontalBox::Slot()

        .FillWidth(1.0f)

        .Padding(
            0.0f,
            0.0f,
            4.0f,
            0.0f
        )

        [

            SNew(SButton)

                .HAlign(HAlign_Center)

                .ContentPadding(
                    FMargin(
                        10.0f,
                        7.0f
                    )
                )

                .IsEnabled_Lambda(
                    []()
                    {
                        return
                            !bIsGenerating;
                    }
                )

                .Text_Lambda(
                    []()
                    {
                        return
                            bIsGenerating
                            ? LOCTEXT(
                                "GeneratingButton",
                                "Generating..."
                            )
                            : LOCTEXT(
                                "GenerateButton",
                                "Generate"
                            );
                    }
                )

                .OnClicked_Static(
                    &FPlayTradeXAbiConverter::Generate
                )

        ]


    // ========================================================
    // Cancel
    // ========================================================

    +SHorizontalBox::Slot()

        .FillWidth(1.0f)

        .Padding(
            4.0f,
            0.0f,
            0.0f,
            0.0f
        )

        [

            SNew(SButton)

                .HAlign(HAlign_Center)

                .ContentPadding(
                    FMargin(
                        10.0f,
                        7.0f
                    )
                )

                .Text(
                    LOCTEXT(
                        "CancelButton",
                        "Cancel"
                    )
                )

                .OnClicked_Static(
                    &FPlayTradeXAbiConverter::Cancel
                )

        ];
}


// ============================================================
// Generate
// ============================================================

FReply FPlayTradeXAbiConverter::Generate()
{
    if (bIsGenerating)
    {
        return FReply::Handled();
    }


    FString ValidationError;

    if (!ValidateInput(
        ValidationError
    ))
    {
        ShowError(
            ValidationError
        );

        return FReply::Handled();
    }


    const FString Abi =
        AbiTextBox->GetText().ToString();


    bIsGenerating =
        true;


    PlayTradeX::SDK::HumanReadableAbi(
        TCHAR_TO_UTF8(
            *Abi
        ),
        false,
        [](
            const PlayTradeX::SDK::
            HumanReadableAbiResponse& Response
            )
        {
            const bool bSuccess =
                Response.success != 0;

            const int32 ErrorCode =
                Response.errorCode;

            const FString HumanReadableAbi =
                UTF8_TO_TCHAR(
                    Response.abi.c_str()
                );

            const FString ErrorMessage =
                UTF8_TO_TCHAR(
                    Response.errorMessage.c_str()
                );


            AsyncTask(
                ENamedThreads::GameThread,
                [
                    bSuccess,
                    ErrorCode,
                    HumanReadableAbi,
                    ErrorMessage
                ]()
                {
                    HandleHumanReadableAbi(
                        bSuccess,
                        ErrorCode,
                        HumanReadableAbi,
                        ErrorMessage
                    );
                }
            );
        }
    );


    return FReply::Handled();
}


// ============================================================
// Cancel
// ============================================================

FReply FPlayTradeXAbiConverter::Cancel()
{
    if (Window.IsValid())
    {
        const TSharedPtr<SWindow> ExistingWindow =
            Window.Pin();

        if (ExistingWindow.IsValid())
        {
            ExistingWindow->RequestDestroyWindow();
        }
    }


    return FReply::Handled();
}


// ============================================================
// Human Readable ABI Callback
// ============================================================

void FPlayTradeXAbiConverter::HandleHumanReadableAbi(
    bool bSuccess,
    int32 ErrorCode,
    const FString& HumanReadableAbi,
    const FString& ErrorMessage
)
{
    bIsGenerating =
        false;


    if (!bSuccess)
    {
        ShowError(
            FString::Printf(
                TEXT(
                    "Failed to generate human readable ABI.\n\n"
                    "Error Code: %d\n"
                    "Message: %s"
                ),
                ErrorCode,
                *ErrorMessage
            )
        );

        return;
    }


    TArray<FString> ParsedHumanReadableAbi;

    if (!ParseHumanReadableAbi(
        HumanReadableAbi,
        ParsedHumanReadableAbi
    ))
    {
        ShowError(
            TEXT(
                "The PlayTradeX SDK returned an empty or invalid "
                "human readable ABI."
            )
        );

        return;
    }


    if (!GenerateContractHeader(
        ParsedHumanReadableAbi
    ))
    {
        return;
    }


    ShowSuccess(
        GetOutputPath()
    );
}


// ============================================================
// Validate Input
// ============================================================

bool FPlayTradeXAbiConverter::ValidateInput(
    FString& OutError
)
{
    if (
        !AbiTextBox.IsValid() ||
        !AddressTextBox.IsValid() ||
        !FileNameTextBox.IsValid()
        )
    {
        OutError =
            TEXT(
                "The ABI Converter UI is not available."
            );

        return false;
    }


    const FString Abi =
        AbiTextBox->GetText().ToString();

    const FString Address =
        AddressTextBox->GetText().ToString();

    const FString FileName =
        FileNameTextBox->GetText().ToString();


    if (Abi.TrimStartAndEnd().IsEmpty())
    {
        OutError =
            TEXT(
                "Please paste a contract ABI."
            );

        return false;
    }


    if (!IsValidJson(
        Abi
    ))
    {
        OutError =
            TEXT(
                "The contract ABI is not valid JSON."
            );

        return false;
    }


    if (FileName.TrimStartAndEnd().IsEmpty())
    {
        OutError =
            TEXT(
                "Please enter a filename."
            );

        return false;
    }


    if (!IsValidCppIdentifier(
        FileName
    ))
    {
        OutError =
            TEXT(
                "The filename must also be a valid C++ identifier."
            );

        return false;
    }


    if (!IsValidEthereumAddress(
        Address
    ))
    {
        OutError =
            TEXT(
                "Please enter a valid EVM contract address."
            );

        return false;
    }


    return true;
}


// ============================================================
// JSON Validation
// ============================================================

bool FPlayTradeXAbiConverter::IsValidJson(
    const FString& Value
)
{
    TSharedRef<TJsonReader<>> Reader =
        TJsonReaderFactory<>::Create(
            Value
        );

    TArray<TSharedPtr<FJsonValue>> Array;


    if (
        FJsonSerializer::Deserialize(
            Reader,
            Array
        )
        )
    {
        return true;
    }


    TSharedRef<TJsonReader<>> ObjectReader =
        TJsonReaderFactory<>::Create(
            Value
        );

    TSharedPtr<FJsonObject> Object;


    return
        FJsonSerializer::Deserialize(
            ObjectReader,
            Object
        ) &&
        Object.IsValid();
}


// ============================================================
// Ethereum Address Validation
// ============================================================

bool FPlayTradeXAbiConverter::IsValidEthereumAddress(
    const FString& Address
)
{
    if (Address.Len() != 42)
    {
        return false;
    }


    if (!Address.StartsWith(
        TEXT("0x"),
        ESearchCase::IgnoreCase
    ))
    {
        return false;
    }


    for (int32 Index = 2;
        Index < Address.Len();
        ++Index)
    {
        const TCHAR Character =
            Address[Index];

        const bool bHex =
            (
                Character >= TEXT('0') &&
                Character <= TEXT('9')
                ) ||
            (
                Character >= TEXT('a') &&
                Character <= TEXT('f')
                ) ||
            (
                Character >= TEXT('A') &&
                Character <= TEXT('F')
                );


        if (!bHex)
        {
            return false;
        }
    }


    return true;
}


// ============================================================
// C++ Identifier Validation
// ============================================================

bool FPlayTradeXAbiConverter::IsValidCppIdentifier(
    const FString& Value
)
{
    if (Value.IsEmpty())
    {
        return false;
    }


    const TCHAR First =
        Value[0];


    if (
        !FChar::IsAlpha(
            First
        ) &&
        First != TEXT('_')
        )
    {
        return false;
    }


    for (int32 Index = 1;
        Index < Value.Len();
        ++Index)
    {
        const TCHAR Character =
            Value[Index];


        if (
            !FChar::IsAlnum(
                Character
            ) &&
            Character != TEXT('_')
            )
        {
            return false;
        }
    }


    return true;
}


// ============================================================
// Parse Human Readable ABI
// ============================================================

bool FPlayTradeXAbiConverter::ParseHumanReadableAbi(
    const FString& Json,
    TArray<FString>& OutAbi
)
{
    OutAbi.Reset();


    TSharedRef<TJsonReader<>> Reader =
        TJsonReaderFactory<>::Create(
            Json
        );

    TArray<TSharedPtr<FJsonValue>> Values;


    if (!FJsonSerializer::Deserialize(
        Reader,
        Values
    ))
    {
        return false;
    }


    for (
        const TSharedPtr<FJsonValue>& Value :
        Values
        )
    {
        if (!Value.IsValid())
        {
            continue;
        }


        FString Entry;


        if (Value->TryGetString(
            Entry
        ))
        {
            if (!Entry.TrimStartAndEnd().IsEmpty())
            {
                OutAbi.Add(
                    Entry
                );
            }
        }
    }


    return
        OutAbi.Num() > 0;
}


// ============================================================
// Function Identifier
// ============================================================

bool FPlayTradeXAbiConverter::TryCreateFunctionEntry(
    const FString& Raw,
    FString& OutIdentifier
)
{
    OutIdentifier.Empty();


    const FString Trimmed =
        Raw.TrimStartAndEnd();


    if (!Trimmed.StartsWith(
        TEXT("function "),
        ESearchCase::CaseSensitive
    ))
    {
        return false;
    }


    const FString FunctionPrefix =
        TEXT("function ");


    const int32 ArgumentStartIndex =
        Trimmed.Find(
            TEXT("("),
            ESearchCase::CaseSensitive
        );


    if (ArgumentStartIndex == INDEX_NONE)
    {
        return false;
    }


    const int32 ArgumentEndIndex =
        Trimmed.Find(
            TEXT(")"),
            ESearchCase::CaseSensitive,
            ESearchDir::FromStart,
            ArgumentStartIndex + 1
        );


    if (
        ArgumentEndIndex == INDEX_NONE ||
        ArgumentEndIndex <
        ArgumentStartIndex
        )
    {
        return false;
    }


    FString FunctionName =
        Trimmed.Mid(
            FunctionPrefix.Len(),
            ArgumentStartIndex -
            FunctionPrefix.Len()
        );

    FunctionName.TrimStartAndEndInline();


    if (FunctionName.IsEmpty())
    {
        return false;
    }


    FString RawArguments =
        Trimmed.Mid(
            ArgumentStartIndex + 1,
            ArgumentEndIndex -
            ArgumentStartIndex -
            1
        );


    FString ArgumentSuffix;

    int32 ArgumentCount =
        0;


    if (!RawArguments.TrimStartAndEnd().IsEmpty())
    {
        TArray<FString> Arguments;

        RawArguments.ParseIntoArray(
            Arguments,
            TEXT(","),
            false
        );


        ArgumentCount =
            Arguments.Num();


        for (FString Argument : Arguments)
        {
            Argument.TrimStartAndEndInline();


            if (Argument.IsEmpty())
            {
                continue;
            }


            TArray<FString> ArgumentParts;

            Argument.ParseIntoArrayWS(
                ArgumentParts
            );


            if (ArgumentParts.Num() == 0)
            {
                continue;
            }


            const FString TypeName =
                NormalizeTypeName(
                    ArgumentParts[0]
                );


            if (!ArgumentSuffix.IsEmpty())
            {
                ArgumentSuffix +=
                    TEXT("_");
            }


            ArgumentSuffix +=
                TypeName;
        }
    }


    OutIdentifier =
        FString::Printf(
            TEXT("%s_%d"),
            *FunctionName,
            ArgumentCount
        );


    if (!ArgumentSuffix.IsEmpty())
    {
        OutIdentifier +=
            TEXT("_");

        OutIdentifier +=
            ArgumentSuffix;
    }


    return true;
}


// ============================================================
// Normalize Solidity Type
// ============================================================

FString FPlayTradeXAbiConverter::NormalizeTypeName(
    const FString& TypeName
)
{
    FString Result =
        TypeName;


    Result.ReplaceInline(
        TEXT("[]"),
        TEXT("Array"),
        ESearchCase::CaseSensitive
    );

    Result.ReplaceInline(
        TEXT("["),
        TEXT("_"),
        ESearchCase::CaseSensitive
    );

    Result.ReplaceInline(
        TEXT("]"),
        TEXT(""),
        ESearchCase::CaseSensitive
    );


    if (!Result.IsEmpty())
    {
        Result[0] =
            FChar::ToUpper(
                Result[0]
            );
    }


    return Result;
}


// ============================================================
// Generate Contract Header
// ============================================================

bool FPlayTradeXAbiConverter::GenerateContractHeader(
    const TArray<FString>& HumanReadableAbi
)
{
    if (
        !AbiTextBox.IsValid() ||
        !AddressTextBox.IsValid() ||
        !FileNameTextBox.IsValid()
        )
    {
        ShowError(
            TEXT(
                "The ABI Converter window is no longer available."
            )
        );

        return false;
    }


    const FString RawAbi =
        AbiTextBox->GetText().ToString();

    const FString Address =
        AddressTextBox->GetText().ToString();

    const FString FileName =
        FileNameTextBox->GetText().ToString();


    // ========================================================
    // Build Function Dictionary
    // ========================================================

    TMap<FString, FString> Functions;


    for (
        const FString& Raw :
        HumanReadableAbi
        )
    {
        FString Identifier;


        if (!TryCreateFunctionEntry(
            Raw,
            Identifier
        ))
        {
            continue;
        }


        if (Functions.Contains(
            Identifier
        ))
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT(
                    "PlayTradeX: Duplicate generated ABI "
                    "identifier '%s' was skipped."
                ),
                *Identifier
            );

            continue;
        }


        Functions.Add(
            Identifier,
            Raw
        );
    }


    // ========================================================
    // Directory
    // ========================================================

    const FString Directory =
        GetGeneratedContractsDirectory();


    if (!IFileManager::Get().DirectoryExists(
        *Directory
    ))
    {
        if (!IFileManager::Get().MakeDirectory(
            *Directory,
            true
        ))
        {
            ShowError(
                FString::Printf(
                    TEXT(
                        "Failed to create generated contracts directory:\n\n%s"
                    ),
                    *Directory
                )
            );

            return false;
        }
    }


    // ========================================================
    // Generate Source
    // ========================================================

    FString Output;


    Output +=
        TEXT("#pragma once\n\n");

    Output +=
        TEXT("#include \"CoreMinimal.h\"\n\n");

    Output +=
        TEXT("namespace ");

    Output +=
        FileName;

    Output +=
        TEXT("\n{\n");


    for (
        const TPair<FString, FString>& Entry :
        Functions
        )
    {
        Output +=
            TEXT("    inline const FString ");

        Output +=
            Entry.Key;

        Output +=
            TEXT(" = TEXT(\"");

        Output +=
            EscapeCppString(
                Entry.Value
            );

        Output +=
            TEXT("\");\n");
    }


    Output +=
        TEXT("\n");

    Output +=
        TEXT("    inline const FString Address = TEXT(\"");

    Output +=
        EscapeCppString(
            Address
        );

    Output +=
        TEXT("\");\n");


    // ========================================================
    // Raw ABI
    // ========================================================

    if (bSaveAbi)
    {
        Output +=
            TEXT("\n");

        Output +=
            TEXT("    inline const FString ABI = TEXT(R\"PTX(");

        Output +=
            EscapeCppRawString(
                RawAbi
            );

        Output +=
            TEXT(")PTX\");\n");
    }


    // ========================================================
    // Human Readable ABI
    // ========================================================

    if (bSaveHumanReadableAbi)
    {
        FString HumanReadableJson =
            TEXT("[\n");


        for (
            int32 Index = 0;
            Index < HumanReadableAbi.Num();
            ++Index
            )
        {
            HumanReadableJson +=
                TEXT("  \"");

            HumanReadableJson +=
                HumanReadableAbi[Index]
                .Replace(
                    TEXT("\\"),
                    TEXT("\\\\")
                )
                .Replace(
                    TEXT("\""),
                    TEXT("\\\"")
                );

            HumanReadableJson +=
                TEXT("\"");


            if (
                Index <
                HumanReadableAbi.Num() - 1
                )
            {
                HumanReadableJson +=
                    TEXT(",");
            }


            HumanReadableJson +=
                TEXT("\n");
        }


        HumanReadableJson +=
            TEXT("]");


        Output +=
            TEXT("\n");

        Output +=
            TEXT("    inline const FString HBI = TEXT(R\"PTX(");

        Output +=
            EscapeCppRawString(
                HumanReadableJson
            );

        Output +=
            TEXT(")PTX\");\n");
    }


    Output +=
        TEXT("}\n");


    // ========================================================
    // Save
    // ========================================================

    const FString OutputPath =
        GetOutputPath();


    if (!FFileHelper::SaveStringToFile(
        Output,
        *OutputPath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM
    ))
    {
        ShowError(
            FString::Printf(
                TEXT(
                    "Failed to write generated contract interface:\n\n%s"
                ),
                *OutputPath
            )
        );

        return false;
    }


    UE_LOG(
        LogTemp,
        Log,
        TEXT(
            "PlayTradeX: Contract interface generated: %s"
        ),
        *OutputPath
    );


    // ========================================================
    // ADDITIVE: Generate Blueprint Function Library
    // ========================================================

    const FString LibraryName = FileName + TEXT("BlueprintLibrary");
    const FString LibraryHeaderName = LibraryName + TEXT(".h");
    const FString LibrarySourceName = LibraryName + TEXT(".cpp");
    const FString LibraryHeaderPath = FPaths::Combine(Directory, LibraryHeaderName);

    // Generated implementation lives in the same runtime module, not the editor module.
    // Directory is .../Source/PlayTradeX/Public/Generated/Contracts.
    // Go up to the module root before appending Private/Generated/Contracts.
    const FString ModuleDirectory = FPaths::GetPath(
        FPaths::GetPath(FPaths::GetPath(Directory))
    );
    const FString SourceDirectory = FPaths::Combine(
        ModuleDirectory, TEXT("Private"), TEXT("Generated"), TEXT("Contracts")
    );
    const FString LibrarySourcePath = FPaths::Combine(SourceDirectory, LibrarySourceName);

    if (!IFileManager::Get().MakeDirectory(*SourceDirectory, true))
    {
        ShowError(FString::Printf(TEXT("Failed to create Blueprint library source directory:\n%s"), *SourceDirectory));
        return false;
    }

    FString LibraryHeader;
    LibraryHeader += TEXT("#pragma once\n\n");
    LibraryHeader += TEXT("#include \"CoreMinimal.h\"\n");
    LibraryHeader += TEXT("#include \"Kismet/BlueprintFunctionLibrary.h\"\n");
    LibraryHeader += FString::Printf(TEXT("#include \"%s.generated.h\"\n\n"), *LibraryName);
    LibraryHeader += TEXT("UCLASS()\n");
    LibraryHeader += FString::Printf(
        TEXT("class U%s : public UBlueprintFunctionLibrary\n{\n    GENERATED_BODY()\n\npublic:\n"),
        *LibraryName
    );

    FString LibrarySource;
    LibrarySource += FString::Printf(
        TEXT("#include \"Generated/Contracts/%s\"\n"), *LibraryHeaderName
    );
    LibrarySource += FString::Printf(
        TEXT("#include \"Generated/Contracts/%s.h\"\n\n"), *FileName
    );

    // All generated getters are static, BlueprintPure, and return existing constants.
    auto AppendGetter = [&](const FString& MethodName, const FString& ConstantName)
        {
            LibraryHeader += FString::Printf(
                TEXT("\n    UFUNCTION(BlueprintPure, Category = \"PlayTradeX|Contracts|%s\")\n")
                TEXT("    static FString %s();\n"),
                *FileName, *MethodName
            );
            LibrarySource += FString::Printf(
                TEXT("FString U%s::%s()\n{\n    return %s::%s;\n}\n\n"),
                *LibraryName, *MethodName, *FileName, *ConstantName
            );
        };

    AppendGetter(TEXT("GetAddress"), TEXT("Address"));
    if (bSaveAbi)
    {
        AppendGetter(TEXT("GetABI"), TEXT("ABI"));
    }
    if (bSaveHumanReadableAbi)
    {
        AppendGetter(TEXT("GetHBI"), TEXT("HBI"));
    }

    TArray<FString> FunctionIdentifiers;
    Functions.GetKeys(FunctionIdentifiers);
    FunctionIdentifiers.Sort();

    for (const FString& Identifier : FunctionIdentifiers)
    {
        // Preserve the existing overload-safe identifier (e.g. balanceOf_1_Address).
        AppendGetter(TEXT("Get_") + Identifier, Identifier);
    }

    LibraryHeader += TEXT("};\n");

    if (!FFileHelper::SaveStringToFile(
        LibraryHeader, *LibraryHeaderPath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
    {
        ShowError(FString::Printf(TEXT("Failed to write Blueprint library header:\n%s"), *LibraryHeaderPath));
        return false;
    }

    if (!FFileHelper::SaveStringToFile(
        LibrarySource, *LibrarySourcePath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
    {
        ShowError(FString::Printf(TEXT("Failed to write Blueprint library source:\n%s"), *LibrarySourcePath));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("PlayTradeX: Blueprint library generated: %s"), *LibraryHeaderPath);
    UE_LOG(LogTemp, Log, TEXT("PlayTradeX: Blueprint library implementation generated: %s"), *LibrarySourcePath);

    return true;
}


// ============================================================
// Escape C++ String
// ============================================================

FString FPlayTradeXAbiConverter::EscapeCppString(
    const FString& Value
)
{
    FString Result =
        Value;


    Result.ReplaceInline(
        TEXT("\\"),
        TEXT("\\\\")
    );

    Result.ReplaceInline(
        TEXT("\""),
        TEXT("\\\"")
    );

    Result.ReplaceInline(
        TEXT("\r"),
        TEXT("\\r")
    );

    Result.ReplaceInline(
        TEXT("\n"),
        TEXT("\\n")
    );


    return Result;
}


// ============================================================
// Escape Raw String
// ============================================================

FString FPlayTradeXAbiConverter::EscapeCppRawString(
    const FString& Value
)
{
    FString Result =
        Value;


    /*
     * Prevent the ABI from accidentally terminating the
     * generated C++ raw string literal.
     */
    Result.ReplaceInline(
        TEXT(")PTX\""),
        TEXT(")PTX_\"")
    );


    return Result;
}


// ============================================================
// Generated Directory
// ============================================================

FString FPlayTradeXAbiConverter::GetGeneratedContractsDirectory()
{
    const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(
            TEXT("PlayTradeX")
        );


    if (!Plugin.IsValid())
    {
        return FPaths::Combine(
            FPaths::ProjectPluginsDir(),
            TEXT("PlayTradeX"),
            TEXT("Source"),
            TEXT("PlayTradeX"),
            TEXT("Public"),
            TEXT("Generated"),
            TEXT("Contracts")
        );
    }


    return FPaths::Combine(
        Plugin->GetBaseDir(),
        TEXT("Source"),
        TEXT("PlayTradeX"),
        TEXT("Public"),
        TEXT("Generated"),
        TEXT("Contracts")
    );
}


// ============================================================
// Output Path
// ============================================================

FString FPlayTradeXAbiConverter::GetOutputPath()
{
    FString FileName =
        FileNameTextBox.IsValid()
        ? FileNameTextBox->GetText().ToString()
        : TEXT("Contract");


    return FPaths::Combine(
        GetGeneratedContractsDirectory(),
        FileName + TEXT(".h")
    );
}


// ============================================================
// Error Dialog
// ============================================================

void FPlayTradeXAbiConverter::ShowError(
    const FString& Message
)
{
    FMessageDialog::Open(
        EAppMsgType::Ok,
        FText::FromString(
            Message
        ),
        LOCTEXT(
            "ErrorDialogTitle",
            "PlayTradeX ABI Converter"
        )
    );
}


// ============================================================
// Success Dialog
// ============================================================

void FPlayTradeXAbiConverter::ShowSuccess(
    const FString& OutputPath
)
{
    FMessageDialog::Open(
        EAppMsgType::Ok,
        FText::FromString(
            FString::Printf(
                TEXT(
                    "Contract interface generated successfully.\n\n%s"
                ),
                *OutputPath
            )
        ),
        LOCTEXT(
            "SuccessDialogTitle",
            "PlayTradeX ABI Converter"
        )
    );
}


#undef LOCTEXT_NAMESPACE