// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.generated.h"

UENUM()
enum class EGameState : uint8
{
    Standby = 0,
    Welcome,
    Placement,
    Interact,
    Select,
    Warning,
    Inspect
};

UENUM()
enum class EObjectType : uint8
{
    None = 0,
    Display,
    Button
};

UENUM()
enum class EAttributeType : uint8
{
    ObjectName = 0,
    DisplayText,
    ButtonText,
    Display
};

UENUM()
enum class EWidgetType : uint8
{
    Welcome = 0,
    Game,
    Inspect
};
