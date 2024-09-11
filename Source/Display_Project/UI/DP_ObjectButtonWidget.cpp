// Display_Project, all rights reserved.

#include "UI/DP_ObjectButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDP_ObjectButtonWidget::Init(EObjectType ObjectType, const FText& ObjectName, int32 WidgetID)
{
    ButtonText->SetText(ObjectName);
    Type = ObjectType;
    ID = WidgetID;
}

void UDP_ObjectButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);
    check(ButtonText);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void UDP_ObjectButtonWidget::OnClicked()
{
    OnClickedObjectButton.Broadcast(Type, ID);
}
