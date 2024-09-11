// Display_Project, all rights reserved.

#include "World/DP_PlaceableActor.h"
#include "DP_CoreStructures.h"

ADP_PlaceableActor::ADP_PlaceableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);
}

FString ADP_PlaceableActor::GetObjectName() const
{
    if (AttributesMap.Contains(EAttributeType::ObjectName))
    {
        return AttributesMap[EAttributeType::ObjectName].Get<FString>();
    }

    return GetName();
}

void ADP_PlaceableActor::Init(FAttributesDataMap&& Attributes)
{
    AttributesMap = MoveTemp(Attributes);
}
