// Display_Project, all rights reserved.

#include "World/DP_Segment.h"
#include "UI/DP_TextWidget.h"
#include "Components/WidgetComponent.h"

UDP_Segment::UDP_Segment()
{
    SetWidgetSpace(EWidgetSpace::World);
    SetDrawAtDesiredSize(true);
}

void UDP_Segment::Set(TCHAR Character)
{
    TextWidget->SetText(FText::FromString(FString{1, &Character}));
}

void UDP_Segment::Clear()
{
    TextWidget->SetText(FText::GetEmpty());
}

void UDP_Segment::BeginPlay()
{
    Super::BeginPlay();

    TextWidget = Cast<UDP_TextWidget>(GetWidget());
    check(TextWidget);
}
