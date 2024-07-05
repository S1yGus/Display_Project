// Display_Project, all rights reserved.

#include "DP_Display_1.h"
#include "DP_Segment.h"
#include "DP_BaseScrollingAlgorithm.h"
#include "Components/SceneComponent.h"

ADP_Display_1::ADP_Display_1()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    check(RootComponent);

    CREATE_SEGMENT(Segment_1);
}

void ADP_Display_1::BeginPlay()
{
    Super::BeginPlay();

    if (ScrollingAlgorithm = NewObject<UDP_BaseScrollingAlgorithm>(this, ScrollingAlgorithmClass))
    {
        ScrollingAlgorithm->StartScrolling(Segments, DisplayText);
    }
}
