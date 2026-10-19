#include "PlayTradeXUnitsLibrary.h"

namespace PlayTradeXUnitsPrivate
{
    static bool Fail(const FString& Message, FString& OutError)
    {
        OutError = Message;
        return false;
    }
}

bool UPlayTradeXUnitsLibrary::ValidateDecimals(const int32 Decimals, FString& OutError)
{
    if (Decimals < 0)
    {
        OutError = TEXT("Decimals cannot be negative.");
        return false;
    }

    OutError.Reset();
    return true;
}

bool UPlayTradeXUnitsLibrary::IsDigitsOnly(const FString& Value)
{
    for (const TCHAR Character : Value)
    {
        if (Character < TEXT('0') || Character > TEXT('9'))
        {
            return false;
        }
    }

    // An empty fractional part is allowed, matching the source implementation.
    return true;
}

FString UPlayTradeXUnitsLibrary::StripLeadingZeros(const FString& Value)
{
    int32 FirstNonZero = 0;

    while (FirstNonZero < Value.Len() - 1 && Value[FirstNonZero] == TEXT('0'))
    {
        ++FirstNonZero;
    }

    return Value.Mid(FirstNonZero);
}

bool UPlayTradeXUnitsLibrary::ToBaseUnit(
    const FString& InAmount,
    const int32 Decimals,
    FString& OutBaseUnit,
    FString& OutError)
{
    OutBaseUnit.Reset();
    OutError.Reset();

    if (!ValidateDecimals(Decimals, OutError))
    {
        return false;
    }

    if (InAmount.IsEmpty() || InAmount.TrimStartAndEnd().IsEmpty())
    {
        return PlayTradeXUnitsPrivate::Fail(TEXT("Amount cannot be null or empty."), OutError);
    }

    FString Amount = InAmount.TrimStartAndEnd();

    if (Amount.StartsWith(TEXT("-"), ESearchCase::CaseSensitive))
    {
        return PlayTradeXUnitsPrivate::Fail(TEXT("Amount cannot be negative."), OutError);
    }

    if (Amount.StartsWith(TEXT("+"), ESearchCase::CaseSensitive))
    {
        Amount.RightChopInline(1, EAllowShrinking::No);
    }

    if (Amount.IsEmpty())
    {
        return PlayTradeXUnitsPrivate::Fail(TEXT("Amount is invalid."), OutError);
    }

    const int32 DecimalPoint = Amount.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromStart);
    const int32 LastDecimalPoint = Amount.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

    if (DecimalPoint != LastDecimalPoint)
    {
        return PlayTradeXUnitsPrivate::Fail(TEXT("Amount contains multiple decimal points."), OutError);
    }

    FString WholePart;
    FString FractionalPart;

    if (DecimalPoint != INDEX_NONE)
    {
        WholePart = Amount.Left(DecimalPoint);
        FractionalPart = Amount.Mid(DecimalPoint + 1);
    }
    else
    {
        WholePart = Amount;
        FractionalPart.Reset();
    }

    if (WholePart.IsEmpty())
    {
        WholePart = TEXT("0");
    }

    if (!IsDigitsOnly(WholePart) || !IsDigitsOnly(FractionalPart))
    {
        return PlayTradeXUnitsPrivate::Fail(
            TEXT("Amount must contain only decimal digits and an optional decimal point."),
            OutError);
    }

    if (FractionalPart.Len() > Decimals)
    {
        return PlayTradeXUnitsPrivate::Fail(
            FString::Printf(TEXT("Amount contains more than %d decimal places."), Decimals),
            OutError);
    }

    if (FractionalPart.Len() < Decimals)
    {
        FractionalPart.Append(FString::ChrN(Decimals - FractionalPart.Len(), TEXT('0')));
    }

    const FString Combined = WholePart + FractionalPart;
    OutBaseUnit = StripLeadingZeros(Combined);
    OutError.Reset();
    return true;
}

bool UPlayTradeXUnitsLibrary::FromBaseUnit(
    const FString& InAmount,
    const int32 Decimals,
    FString& OutAmount,
    FString& OutError)
{
    OutAmount.Reset();
    OutError.Reset();

    if (!ValidateDecimals(Decimals, OutError))
    {
        return false;
    }

    if (InAmount.IsEmpty() || InAmount.TrimStartAndEnd().IsEmpty())
    {
        return PlayTradeXUnitsPrivate::Fail(TEXT("Amount cannot be null or empty."), OutError);
    }

    FString Amount = InAmount.TrimStartAndEnd();
    bool bNegative = false;

    if (Amount.StartsWith(TEXT("-"), ESearchCase::CaseSensitive))
    {
        bNegative = true;
        Amount.RightChopInline(1, EAllowShrinking::No);
    }
    else if (Amount.StartsWith(TEXT("+"), ESearchCase::CaseSensitive))
    {
        Amount.RightChopInline(1, EAllowShrinking::No);
    }

    if (Amount.IsEmpty() || !IsDigitsOnly(Amount))
    {
        return PlayTradeXUnitsPrivate::Fail(TEXT("Base-unit amount must be an integer."), OutError);
    }

    const FString Digits = StripLeadingZeros(Amount);
    const bool bIsZero = Digits == TEXT("0");

    if (Decimals == 0)
    {
        OutAmount = (bNegative && !bIsZero) ? TEXT("-") + Digits : Digits;
        return true;
    }

    FString PaddedDigits = Digits;
    if (PaddedDigits.Len() <= Decimals)
    {
        PaddedDigits = FString::ChrN(Decimals + 1 - PaddedDigits.Len(), TEXT('0')) + PaddedDigits;
    }

    const int32 DecimalPosition = PaddedDigits.Len() - Decimals;
    const FString WholePart = PaddedDigits.Left(DecimalPosition);
    FString FractionalPart = PaddedDigits.Mid(DecimalPosition);

    while (FractionalPart.EndsWith(TEXT("0"), ESearchCase::CaseSensitive))
    {
        FractionalPart.LeftChopInline(1, EAllowShrinking::No);
    }

    OutAmount = FractionalPart.IsEmpty()
        ? WholePart
        : WholePart + TEXT(".") + FractionalPart;

    if (bNegative && !bIsZero)
    {
        OutAmount = TEXT("-") + OutAmount;
    }

    return true;
}

bool UPlayTradeXUnitsLibrary::ToWei(
    const FString& Amount,
    FString& OutWei,
    FString& OutError)
{
    return ToBaseUnit(Amount, WeiDecimals, OutWei, OutError);
}

bool UPlayTradeXUnitsLibrary::FromWei(
    const FString& Wei,
    FString& OutAmount,
    FString& OutError)
{
    return FromBaseUnit(Wei, WeiDecimals, OutAmount, OutError);
}
