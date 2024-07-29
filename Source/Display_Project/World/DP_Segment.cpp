// Display_Project, all rights reserved.

#include "World/DP_Segment.h"
#include "UI/DP_SegmentWidget.h"
#include "Components/WidgetComponent.h"

UDP_Segment::UDP_Segment()
{
    SetWidgetSpace(EWidgetSpace::World);
    SetDrawAtDesiredSize(true);
}

void UDP_Segment::Set(TCHAR Character)
{
    SegmentWidget->SetText(FText::FromString(FString{1, &Character}));
}

void UDP_Segment::Clear()
{
    SegmentWidget->SetText(FText::GetEmpty());
}

void UDP_Segment::BeginPlay()
{
    Super::BeginPlay();

    SegmentWidget = Cast<UDP_SegmentWidget>(GetWidget());
    check(SegmentWidget);
}
