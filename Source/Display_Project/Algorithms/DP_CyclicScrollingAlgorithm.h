// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_BaseScrollingAlgorithm.h"
#include "DP_CyclicScrollingAlgorithm.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract)
class DISPLAY_PROJECT_API UDP_CyclicScrollingAlgorithm : public UDP_BaseScrollingAlgorithm
{
    GENERATED_BODY()

public:
    virtual void StartScrolling(const TArray<TWeakInterfacePtr<IDP_SegmentInterface>>& InSegments, const FString& InText) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0.0", Units = "s"))
    float ScrollingRate{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
    int32 AdditionalSpaces{0};

private:
    FTimerHandle ScrollingTimerHandle;
    FString Text;
    uint32 TextIndex{0};
    TArray<TWeakInterfacePtr<IDP_SegmentInterface>> Segments;

    void OnScrolling();
};
