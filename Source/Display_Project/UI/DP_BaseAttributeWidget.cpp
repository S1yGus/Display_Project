// Display_Project, all rights reserved.

#include "UI/DP_BaseAttributeWidget.h"
#include "Components/TextBlock.h"

void UDP_BaseAttributeWidget::Init(EAttributeType AttributeType, const FAttribute& Attribute)
{
    Type = AttributeType;
    AttributeName->SetText(Attribute.Name);
}

void UDP_BaseAttributeWidget::Reset()
{
}

void UDP_BaseAttributeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(AttributeName);
}
