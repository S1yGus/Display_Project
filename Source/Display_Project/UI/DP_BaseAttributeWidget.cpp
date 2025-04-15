// Display_Project, all rights reserved.

#include "UI/DP_BaseAttributeWidget.h"
#include "Components/TextBlock.h"
#include "DP_Utils.h"

void UDP_BaseAttributeWidget::Init(EAttributeType AttributeType)
{
    Type = AttributeType;
    AttributeName->SetText(UI::AttributeTypeToText(AttributeType));
}

void UDP_BaseAttributeWidget::Update(const TAttributeData& Data)
{
}

void UDP_BaseAttributeWidget::Reset()
{
}

void UDP_BaseAttributeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(AttributeName);
}
