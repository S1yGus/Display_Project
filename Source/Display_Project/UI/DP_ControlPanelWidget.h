// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreDelegates.h"
#include "DP_ControlPanelWidget.generated.h"

class UDP_ControlPanelButtonWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_ControlPanelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnQuitSignature OnQuit;
    FOnToggleScreenModeSignature OnToggleScreenMode;
    FOnShowOptionsSignature OnShowOptions;
    FOnShowHelpSignature OnShowHelp;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDP_ControlPanelButtonWidget> QuitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDP_ControlPanelButtonWidget> ToggleScreenModeButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDP_ControlPanelButtonWidget> OptionsButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDP_ControlPanelButtonWidget> HelpButton;

    virtual void NativeOnInitialized() override;

private:
    void OnQuitHandler();
    void OnToggleScreenModeHandler();
    void OnShowOptionsHandler();
    void OnShowHelpHandler();
};
