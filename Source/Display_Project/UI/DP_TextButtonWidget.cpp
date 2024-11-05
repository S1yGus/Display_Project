// Display_Project, all rights reserved.

#include "UI/DP_TextButtonWidget.h"
#include "Components/TextBlock.h"

void UDP_TextButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ButtonText);
}

void UDP_TextButtonWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ButtonText)
    {
        ButtonText->SetText(Text);
    }
}
