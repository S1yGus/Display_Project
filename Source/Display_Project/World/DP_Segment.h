// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/DP_SegmentInterface.h"
#include "DP_Segment.generated.h"

class UDP_TextWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DISPLAY_PROJECT_API UDP_Segment : public UWidgetComponent, public IDP_SegmentInterface
{
    GENERATED_BODY()

public:
    UDP_Segment();

    virtual void Set(TCHAR Character) override;
    virtual void Clear() override;

    virtual void BeginPlay() override;

private:
    TObjectPtr<UDP_TextWidget> TextWidget;
};
