// Display_Project, all rights reserved.

#include "World/DP_Node.h"
#include "Components/BoxComponent.h"
#include "World/DP_PlaceableActor.h"

#define ECC_Node ECollisionChannel::ECC_GameTraceChannel2

ADP_Node::ADP_Node()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    check(RootComponent);

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollison");
    check(BoxCollision);
    BoxCollision->SetCollisionResponseToChannel(ECC_Node, ECollisionResponse::ECR_Block);
    BoxCollision->SetupAttachment(GetRootComponent());
}

void ADP_Node::Occupy(ADP_PlaceableActor* NewOccupiedObject)
{
    OccupiedObject = NewOccupiedObject;

    UpdateOccupiedState(true);
}

void ADP_Node::Free()
{
    if (IsValid(OccupiedObject))
    {
        OccupiedObject->Destroy();
    }

    UpdateOccupiedState(false);
}

bool ADP_Node::IsOccupied() const
{
    return IsValid(OccupiedObject);
}

void ADP_Node::UpdateCollisionScale(const FVector& CollisionSize)
{
    BoxCollision->SetBoxExtent(CollisionSize / 2);
}
