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

AActor* GetPlaceableActorByGuid(const UObject* WorldContextObject, const FGuid& Guid);

template <class T>
T* GetPlaceableActorByGuid(const UObject* WorldContextObject, const FGuid& Guid)
{
    return Cast<T>(GetPlaceableActorByGuid(WorldContextObject, Guid));
}

}    // namespace DP

namespace UI
{

uint64 UniqueID();
FText GenerateUniqueObjectName(const FText& Name);
FText GetAdjustedDefaultText(EAttributeType Type, const FText& Text);
FText ObjectTypeToText(EObjectType Type);
FText AttributeTypeToText(EAttributeType Type);
EWidgetType GameStateToWidgetType(EGameState GameState);

}    // namespace UI
