// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "DP_SegmentInterface.h"
#include "DP_Segment.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DISPLAY_PROJECT_API UDP_Segment : public UTextRenderComponent, public IDP_SegmentInterface
{
    GENERATED_BODY()

public:
    UDP_Segment();

    virtual void Set(TCHAR Character) override;
    virtual void Clear() override;
};
