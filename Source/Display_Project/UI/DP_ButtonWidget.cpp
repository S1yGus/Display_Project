// Display_Project, all rights reserved.

#include "UI/DP_ButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UDP_ButtonWidget::Init(UTexture2D* InThumbnail)
{
    Thumbnail = InThumbnail;
}

void UDP_ButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);
    check(ButtonBackgroundImage);

    ButtonBackgroundImage->SetColorAndOpacity(UnhoveredTint);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClickedHandler);
    Button->OnHovered.AddDynamic(this, &ThisClass::OnHoveredHandler);
    Button->OnUnhovered.AddDynamic(this, &ThisClass::OnUnhoveredHandler);
    Button->OnPressed.AddDynamic(this, &ThisClass::OnPressedHandler);
}

void UDP_ButtonWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ButtonImage)
    {
        ButtonImage->SetBrushFromTexture(Thumbnail);
    }

    if (ButtonBackgroundImage)
    {
        ButtonBackgroundImage->SetColorAndOpacity(UnhoveredTint);
    }
}

void UDP_ButtonWidget::OnClickedHandler()
{
    ButtonBackgroundImage->SetColorAndOpacity(HoveredTint);
    OnClicked.Broadcast();
}

void UDP_ButtonWidget::OnHoveredHandler()
{
    ButtonBackgroundImage->SetColorAndOpacity(HoveredTint);
}

void UDP_ButtonWidget::OnUnhoveredHandler()
{
    ButtonBackgroundImage->SetColorAndOpacity(UnhoveredTint);
}

void UDP_ButtonWidget::OnPressedHandler()
{
    ButtonBackgroundImage->SetColorAndOpacity(PressedTint);
}
