// Display_Project, all rights reserved.

#include "UI/DP_ControlPanelWidget.h"
#include "UI/DP_ControlPanelButtonWidget.h"

void UDP_ControlPanelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(QuitButton);
    check(ToggleScreenModeButton);
    check(HelpButton);

    QuitButton->OnClicked.AddUObject(this, &ThisClass::OnQuitHandler);
    ToggleScreenModeButton->OnClicked.AddUObject(this, &ThisClass::OnToggleScreenModeHandler);
    HelpButton->OnClicked.AddUObject(this, &ThisClass::OnShowHelpHandler);
}

void UDP_ControlPanelWidget::OnQuitHandler()
{
    OnQuit.Broadcast();
}

void UDP_ControlPanelWidget::OnToggleScreenModeHandler()
{
    OnToggleScreenMode.Broadcast();
}

void UDP_ControlPanelWidget::OnShowHelpHandler()
{
    OnShowHelp.Broadcast();
}
