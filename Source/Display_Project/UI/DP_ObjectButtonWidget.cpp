// Display_Project, all rights reserved.

#include "UI/DP_ObjectButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DP_Utils.h"

void UDP_ObjectButtonWidget::Init(EObjectType ObjectType, int32 WidgetID)
{
    ButtonText->SetText(UI::ObjectTypeToText(ObjectType));
    Type = ObjectType;
    ID = WidgetID;
}

void UDP_ObjectButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);
    check(ButtonText);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClickedHandler);
}

void UDP_ObjectButtonWidget::OnClickedHandler()
{
    OnClickedObjectButton.Broadcast(Type, ID);
}
