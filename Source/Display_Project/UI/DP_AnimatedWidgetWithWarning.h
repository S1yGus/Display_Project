// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_BaseAnimatedWidget.h"
#include "DP_AnimatedWidgetWithWarning.generated.h"

class UDP_WarningWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_AnimatedWidgetWithWarning : public UDP_BaseAnimatedWidget
{
    GENERATED_BODY()

public:
    FOnWarningResponseSignature OnWarningResponse;

    void ShowWarning(const FText& WarningText);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_WarningWidget> WarningWidget;

    virtual void NativeOnInitialized() override;

private:
    void OnWarningResponseHandler(bool bCondition);
    void OnWarningFadeoutAnimationFinishedHandler();
};
