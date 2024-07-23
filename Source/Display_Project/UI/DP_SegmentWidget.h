// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_SegmentWidget.generated.h"

class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_SegmentWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetText(const FText& Text);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> SegmentText;
};
