// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DP_BaseScrollingAlgorithm.generated.h"

class IDP_SegmentInterface;

UCLASS(Abstract)
class DISPLAY_PROJECT_API UDP_BaseScrollingAlgorithm : public UObject
{
    GENERATED_BODY()

public:
    virtual void StartScrolling(const TArray<TWeakInterfacePtr<IDP_SegmentInterface>>& InSegments, const FString& InText);
};
