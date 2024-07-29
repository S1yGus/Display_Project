// Display_Project, all rights reserved.

#include "World/DP_Display_1.h"
#include "World/DP_Segment.h"
#include "Algorithms/DP_BaseScrollingAlgorithm.h"
#include "Components/SceneComponent.h"

ADP_Display_1::ADP_Display_1()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    check(RootComponent);

    CREATE_SEGMENT(Segment_1);
}

void ADP_Display_1::RefreshText(const FString& InText)
{
    DisplayText = InText;
    ScrollingAlgorithm->RefreshText(DisplayText);
}

void ADP_Display_1::BeginPlay()
{
    Super::BeginPlay();

    ScrollingAlgorithm = NewObject<UDP_BaseScrollingAlgorithm>(this, ScrollingAlgorithmClass);
    check(ScrollingAlgorithm);
    ScrollingAlgorithm->StartScrolling(Segments, DisplayText);
}
