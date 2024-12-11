// Display_Project, all rights reserved.

#include "UI/DP_ControlPanelWidget.h"
#include "UI/DP_ControlPanelButtonWidget.h"

void UDP_ControlPanelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(QuitButton);
    check(ToggleScreenModeButton);
    check(OptionsButton);
    check(HelpButton);

    QuitButton->OnClicked.AddUObject(this, &ThisClass::OnQuitHandler);
    ToggleScreenModeButton->OnClicked.AddUObject(this, &ThisClass::OnToggleScreenModeHandler);
    OptionsButton->OnClicked.AddUObject(this, &ThisClass::OnShowOptionsHandler);
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

void UDP_ControlPanelWidget::OnShowOptionsHandler()
{
    OnShowOptions.Broadcast();
}

void UDP_ControlPanelWidget::OnShowHelpHandler()
{
    OnShowHelp.Broadcast();
}
