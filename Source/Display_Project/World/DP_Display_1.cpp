// Display_Project, all rights reserved.

#include "World/DP_Display_1.h"
#include "World/DP_Segment.h"
#include "Algorithms/DP_BaseScrollingAlgorithm.h"

DEFINE_LOG_CATEGORY_STATIC(LogDisplay, All, All)

ADP_Display_1::ADP_Display_1()
{
    PrimaryActorTick.bCanEverTick = false;

    CREATE_SEGMENT(Segment_1);
}

void ADP_Display_1::RefreshText(const FString& Text)
{
    DisplayText = Text.ToUpper();
    ScrollingAlgorithm->RefreshText(DisplayText);
}

void ADP_Display_1::Init(FAttributesDataMap&& Attributes)
{
    Super::Init(MoveTemp(Attributes));

    if (AttributesMap.Contains(EAttributeType::DisplayText))
    {
        DisplayText = AttributesMap[EAttributeType::DisplayText].Get<FString>().ToUpper();
    }
}

void ADP_Display_1::BeginPlay()
{
    Super::BeginPlay();

    ScrollingAlgorithm = NewObject<UDP_BaseScrollingAlgorithm>(this, ScrollingAlgorithmClass);
    check(ScrollingAlgorithm);
    ScrollingAlgorithm->StartScrolling(Segments, DisplayText.ToUpper());
}
