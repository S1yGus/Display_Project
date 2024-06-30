// Display_Project, all rights reserved.

#include "DP_Display_1.h"
#include "DP_Segment.h"
#include "DP_SegmentInterface.h"
#include "Components/SceneComponent.h"

ADP_Display_1::ADP_Display_1()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    check(RootComponent);

    CREATE_SEGMENT(Segment_1, "Segment", "1");
}

void ADP_Display_1::BeginPlay()
{
    Super::BeginPlay();

    for (int32 i = 0; i < Segments.Num(); ++i)
    {
        if (Segments[i].IsValid())
        {
            if (i < Text.Len())
            {
                Segments[i]->Set(Text[i]);
            }
            else
            {
                Segments[i]->Clear();
            }
        }
    }
}
