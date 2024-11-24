// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_BaseAnimatedWidget.h"
#include "DP_InspectWidget.generated.h"

class UDP_ButtonWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_InspectWidget : public UDP_BaseAnimatedWidget
{
    GENERATED_BODY()

public:
    FOnInspectCompletedSignature OnInspectCompleted;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> BackButton;

    virtual void NativeOnInitialized() override;

private:
    void OnClickedBackButtonHandler();
};
