// Display_Project, all rights reserved.

#include "Algorithms/DP_CyclicScrollingAlgorithm.h"
#include "Interfaces/DP_SegmentInterface.h"

void UDP_CyclicScrollingAlgorithm::StartScrolling(const TArray<TWeakInterfacePtr<IDP_SegmentInterface>>& InSegments, const FString& InText)
{
    Segments = InSegments;
    RefreshText(InText);
}

void UDP_CyclicScrollingAlgorithm::RefreshText(const FString& InText)
{
    Text = InText;
    TextIndex = 0;

    OnScrolling();

    if (Text.Len() > Segments.Num())
    {
        for (int32 i = 0; i < AdditionalSpaces; ++i)
        {
            Text.AppendChar(' ');
        }
        GetWorld()->GetTimerManager().SetTimer(ScrollingTimerHandle, this, &ThisClass::OnScrolling, ScrollingRate, true);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(ScrollingTimerHandle);
    }
}

void UDP_CyclicScrollingAlgorithm::OnScrolling()
{
    for (int32 i = 0; i < Segments.Num(); ++i)
    {
        if (Segments[i].IsValid())
        {
            if (i < Text.Len())
            {
                Segments[i]->Set(Text[(i + TextIndex) % Text.Len()]);
            }
            else
            {
                Segments[i]->Clear();
            }
        }
    }

    ++TextIndex;
}
