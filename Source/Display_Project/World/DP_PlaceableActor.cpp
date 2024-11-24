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

void ADP_PlaceableActor::Init(FAttributesMap&& Attributes)
{
    AttributesMap = MoveTemp(Attributes);
    UpdateAttributes();
}

void ADP_PlaceableActor::Init(const FAttributesMap& Attributes)
{
    AttributesMap = Attributes;
    UpdateAttributes();
}

void ADP_PlaceableActor::UpdateAttribute(EAttributeType AttributeType, FAttributeData AttributeData)
{
    SetAttribute(AttributeType, AttributeData);
    UpdateAttributes();
}

void ADP_PlaceableActor::UpdateAttributes()
{
}

void ADP_PlaceableActor::SetAttribute(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (!AttributesMap.Contains(AttributeType))
    {
        AttributesMap.Add(AttributeType);
    }
    AttributesMap[AttributeType] = AttributeData;
}
