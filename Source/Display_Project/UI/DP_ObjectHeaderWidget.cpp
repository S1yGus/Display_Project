// Display_Project, all rights reserved.

#include "UI/DP_ObjectHeaderWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DP_Utils.h"

void UDP_ObjectHeaderWidget::Init(EObjectType Type, UTexture2D* InThumbnail)
{
    Thumbnail->SetBrushFromTexture(InThumbnail);
    ObjectType->SetText(UI::ObjectTypeToText(Type));
}

void UDP_ObjectHeaderWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ObjectType);
    check(Thumbnail);
}
