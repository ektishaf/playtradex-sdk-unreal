#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayTradeXUnitsLibrary.generated.h"

/**
 * Exact decimal-string conversions for blockchain token/native-currency amounts.
 *
 * Uses decimal strings instead of floating-point values, preserving precision for
 * large blockchain amounts. Invalid inputs return false and populate OutError.
 */
UCLASS()
class PLAYTRADEX_API UPlayTradeXUnitsLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Converts a human-readable amount into integer base units. Example: "1.5", 18 -> "1500000000000000000". */
    UFUNCTION(BlueprintPure, Category = "PlayTradeX|Units", meta = (DisplayName = "To Base Unit"))
    static bool ToBaseUnit(const FString& Amount, int32 Decimals, FString& OutBaseUnit, FString& OutError);

    /** Converts integer base units into a human-readable amount. Example: "1500000", 6 -> "1.5". */
    UFUNCTION(BlueprintPure, Category = "PlayTradeX|Units", meta = (DisplayName = "From Base Unit"))
    static bool FromBaseUnit(const FString& Amount, int32 Decimals, FString& OutAmount, FString& OutError);

    /** Converts an 18-decimal EVM native-currency amount into wei. */
    UFUNCTION(BlueprintPure, Category = "PlayTradeX|Units", meta = (DisplayName = "To Wei"))
    static bool ToWei(const FString& Amount, FString& OutWei, FString& OutError);

    /** Converts wei into a human-readable 18-decimal EVM native-currency amount. */
    UFUNCTION(BlueprintPure, Category = "PlayTradeX|Units", meta = (DisplayName = "From Wei"))
    static bool FromWei(const FString& Wei, FString& OutAmount, FString& OutError);

private:
    static constexpr int32 WeiDecimals = 18;

    static bool ValidateDecimals(int32 Decimals, FString& OutError);
    static bool IsDigitsOnly(const FString& Value);
    static FString StripLeadingZeros(const FString& Value);
};
