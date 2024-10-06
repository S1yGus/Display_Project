// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"
#include "DP_CoreStructures.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectTypeChangedSignature, EObjectType);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickedObjectButtonSignature, EObjectType, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignature, EAttributeType, FAttributeData);
DECLARE_MULTICAST_DELEGATE(FOnRequestObjectSpawnSignature);
DECLARE_MULTICAST_DELEGATE(FOnObjectSpawnedSignature);
DECLARE_MULTICAST_DELEGATE(FOnDestroyAllSignature);
DECLARE_MULTICAST_DELEGATE(FOnDestroySelectedSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatePreviewLocationSignature, AActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectSelectedSignature, AActor*, const FTransform&);
