// Display_Project, all rights reserved.

#include "World/DP_PlaceableActor.h"

ADP_PlaceableActor::ADP_PlaceableActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADP_PlaceableActor::BeginPlay()
{
    Super::BeginPlay();

    OnBeginCursorOver.AddDynamic(this, &ThisClass::OnBeginCursorHover);
    OnEndCursorOver.AddDynamic(this, &ThisClass::OnEndCursorHover);
}
