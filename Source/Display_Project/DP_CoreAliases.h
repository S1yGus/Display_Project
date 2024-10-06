// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"

class ADP_Display_1;
class UComboBoxString;

using FAttributeData = TVariant<FString, TObjectPtr<ADP_Display_1>>;
using FAttributesMap = TMap<EAttributeType, FAttributeData>;
using FComboBoxDataUpdater = TFunction<void(TObjectPtr<UComboBoxString>, TArray<FAttributeData>&)>;
using FComboBoxSelectedOptionGetter = TFunction<FString(const FAttributeData&)>;
