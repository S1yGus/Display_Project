// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"

class UComboBoxString;
class ADP_Node;

using FAttributeData = TVariant<FString, FGuid>;
using FAttributesMap = TMap<EAttributeType, FAttributeData>;
using FComboBoxDataUpdater = TFunction<void(TObjectPtr<UComboBoxString>, TArray<FAttributeData>&)>;
using FComboBoxSelectedOptionGetter = TFunction<FString(const FAttributeData&)>;
using FDeferredAction = TFunction<void()>;
using FScaleComplete = TFunction<void()>;
using FDOFSensorWidthChangeComplete = TFunction<void()>;
using FOccupiedNodesMap = TMap<FGuid, TSet<TObjectPtr<ADP_Node>>>;
