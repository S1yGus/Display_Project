// Display_Project, all rights reserved.

#include "UI/DP_ControlPanelButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDP_ControlPanelButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);
    check(ButtonText);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClickedHandler);
}

void UDP_ControlPanelButtonWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ButtonText)
    {
        ButtonText->SetText(Text);
        ButtonText->SetColorAndOpacity(ButtonTextColor);
    }
}

void UDP_ControlPanelButtonWidget::OnClickedHandler()
{
    OnClicked.Broadcast();
}
