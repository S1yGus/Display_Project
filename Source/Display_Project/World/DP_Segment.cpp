// Display_Project, all rights reserved.

#include "DP_Segment.h"
#include "Components/TextRenderComponent.h"

UDP_Segment::UDP_Segment()
{
    Text = FText::GetEmpty();
}

void UDP_Segment::Set(TCHAR Character)
{
    SetText(FText::FromString(FString{1, &Character}));
}

void UDP_Segment::Clear()
{
    SetText(FText::GetEmpty());
}
