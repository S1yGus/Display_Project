// Display_Project, all rights reserved.

#include "UI/DP_ObjectHeaderWidget.h"
#include "Components/TextBlock.h"
#include "DP_Utils.h"

void UDP_ObjectHeaderWidget::Init(EObjectType Type)
{
    ObjectType->SetText(UI::ObjectTypeToText(Type));
}

void UDP_ObjectHeaderWidget::UpdateObjectName(const FString& Name)
{
    ObjectName->SetText(FText::FromString(Name));
}

void UDP_ObjectHeaderWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ObjectType);
    check(ObjectName);
}
