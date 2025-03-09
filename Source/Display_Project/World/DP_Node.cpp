// Display_Project, all rights reserved.

#include "World/DP_Node.h"
#include "Components/BoxComponent.h"
#include "DP_Utils.h"

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

void ADP_Node::Occupy(const FGuid& Guid)
{
    OccupyingGuid = Guid;
    UpdateOccupiedState(OccupyingGuid.IsValid());
}

void ADP_Node::Free()
{
    if (!OccupyingGuid.IsValid())
        return;

    if (auto* OccupyingObject = DP::GetPlaceableActorByGuid(GetWorld(), OccupyingGuid); IsValid(OccupyingObject))
    {
        OccupyingObject->Destroy();
    }
    OccupyingGuid.Invalidate();
    UpdateOccupiedState(false);
}

bool ADP_Node::IsOccupied() const
{
    return OccupyingGuid.IsValid();
}

void ADP_Node::UpdateCollisionScale(const FVector& CollisionSize)
{
    BoxCollision->SetBoxExtent(CollisionSize / 2);
}
