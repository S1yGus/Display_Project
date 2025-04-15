// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"

class UComboBoxString;
class ADP_Node;

using TAttributeData = TVariant<FString, FGuid>;
using TAttributesMap = TMap<EAttributeType, TAttributeData>;
using TComboBoxDataUpdater = TFunction<void(TObjectPtr<UComboBoxString>, TArray<TAttributeData>&)>;
using TComboBoxSelectedOptionGetter = TFunction<FString(const TAttributeData&)>;
using TDeferredAction = TFunction<void()>;
using TScaleComplete = TFunction<void()>;
using TDOFSensorWidthChangeComplete = TFunction<void()>;
using TOccupiedNodesMap = TMap<FGuid, TSet<TObjectPtr<ADP_Node>>>;
