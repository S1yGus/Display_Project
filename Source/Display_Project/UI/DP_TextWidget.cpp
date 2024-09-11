// Display_Project, all rights reserved.

#include "UI/DP_TextWidget.h"
#include "Components/TextBlock.h"

void UDP_TextWidget::SetText(const FText& Text)
{
    WidgetText->SetText(Text);
}

void UDP_TextWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(WidgetText);
}
