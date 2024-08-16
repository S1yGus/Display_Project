// Display_Project, all rights reserved.

#pragma once

#include "DP_CoreTypes.generated.h"

UENUM()
enum class EGameState : uint8
{
    WaitingToStart = 0,
    ObjectPlacement
};

UENUM()
enum class EObjectType : uint8
{
    Display = 0,
    Button
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
