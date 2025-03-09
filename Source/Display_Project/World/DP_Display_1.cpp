// Display_Project, all rights reserved.

#include "World/DP_Display_1.h"
#include "World/DP_Segment.h"
#include "Algorithms/DP_BaseScrollingAlgorithm.h"

DEFINE_LOG_CATEGORY_STATIC(LogDisplay, All, All)

ADP_Display_1::ADP_Display_1()
{
    PrimaryActorTick.bCanEverTick = false;
    Type = EObjectType::Display;

    CREATE_SEGMENT(Segment_1);
}

void ADP_Display_1::BeginPlay()
{
    Super::BeginPlay();

    ScrollingAlgorithm = NewObject<UDP_BaseScrollingAlgorithm>(this, ScrollingAlgorithmClass);
    check(ScrollingAlgorithm);
    ScrollingAlgorithm->StartScrolling(Segments, DisplayText.ToUpper());
}

void ADP_Display_1::UpdateAttributes()
{
    Super::UpdateAttributes();

    if (AttributesMap.Contains(EAttributeType::DisplayText))
    {
        RefreshText(AttributesMap[EAttributeType::DisplayText].Get<FString>());
    }
}

void ADP_Display_1::RefreshText(const FString& Text)
{
    if (IsValid(ScrollingAlgorithm))
    {
        DisplayText = Text.ToUpper();
        ScrollingAlgorithm->RefreshText(DisplayText);
    }
}
