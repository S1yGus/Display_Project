// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreTypes.h"

namespace DP
{

template <class T>
FString EnumToString(T EnumValue)
{
    if (const UEnum* EnumPtr = StaticEnum<T>())
    {
        return EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue));
    }
    return FString("Invalid");
}

}    // namespace DP

namespace UI
{

uint64 UniqueID();
FText ObjectTypeToText(EObjectType Type);
FText GenerateUniqueObjectName(const FText& Name);

}    // namespace UI