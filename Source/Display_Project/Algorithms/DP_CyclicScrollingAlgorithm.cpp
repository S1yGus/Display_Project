// Display_Project, all rights reserved.

#include "DP_CyclicScrollingAlgorithm.h"
#include "DP_SegmentInterface.h"

void UDP_CyclicScrollingAlgorithm::StartScrolling(const TArray<TWeakInterfacePtr<IDP_SegmentInterface>>& InSegments, const FString& InText)
{
    Segments = InSegments;
    Text = InText;

    OnScrolling();

    if (Text.Len() > Segments.Num())
    {
        for (int32 i = 0; i < AdditionalSpaces; ++i)
        {
            Text.AppendChar(' ');
        }
        GetWorld()->GetTimerManager().SetTimer(ScrollingTimerHandle, this, &ThisClass::OnScrolling, ScrollingRate, true);
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
