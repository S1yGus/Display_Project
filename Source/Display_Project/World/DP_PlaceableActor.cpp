// Display_Project, all rights reserved.

#include "World/DP_PlaceableActor.h"
#include "Components/DP_FXComponent.h"
#include "DP_CoreStructures.h"

ADP_PlaceableActor::ADP_PlaceableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    FXComponent = CreateDefaultSubobject<UDP_FXComponent>("FXComponent");
    check(FXComponent);
}

FString ADP_PlaceableActor::GetObjectName() const
{
    if (AttributesMap.Contains(EAttributeType::ObjectName))
    {
        return AttributesMap[EAttributeType::ObjectName].Get<FString>();
    }
    return GetName();
}

void ADP_PlaceableActor::Init(FAttributesMap&& Attributes, FGuid&& InGuid)
{
    AttributesMap = MoveTemp(Attributes);
    Guid = MoveTemp(InGuid);
    UpdateAttributes();
}

void ADP_PlaceableActor::Init(const FAttributesMap& Attributes, const FGuid& InGuid)
{
    AttributesMap = Attributes;
    Guid = InGuid;
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

void ADP_PlaceableActor::Interact(const FTransform& InteractionTransform)
{
    FXComponent->MakeInteractFX(InteractionTransform);
}

void ADP_PlaceableActor::SetAttribute(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (!AttributesMap.Contains(AttributeType))
    {
        AttributesMap.Add(AttributeType);
    }
    AttributesMap[AttributeType] = AttributeData;
}
