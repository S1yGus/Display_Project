// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"
#include "DP_CoreStructures.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectTypeChangedSignature, EObjectType);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickedObjectButtonSignature, EObjectType, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignature, EAttributeType, FAttributeData);
