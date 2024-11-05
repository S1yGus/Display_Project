// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_BaseAnimatedWidget.h"
#include "DP_CoreDelegates.h"
#include "DP_WarningWidget.generated.h"

class UDP_ButtonWidget;
class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_WarningWidget : public UDP_BaseAnimatedWidget
{
    GENERATED_BODY()

public:
    FOnWarningResponseSignature OnWarningResponse;

    void SetWarning(const FText& InWarningText);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> WarningText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> AcceptButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> DenyButton;

    virtual void NativeOnInitialized() override;

private:
    void OnClickedAcceptButtonHandler();
    void OnClickedDenyButtonHandler();
};
