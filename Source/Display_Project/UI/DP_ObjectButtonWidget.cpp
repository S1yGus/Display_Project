// Display_Project, all rights reserved.

#include "UI/DP_ObjectButtonWidget.h"
#include "UI/DP_ButtonWidget.h"
#include "Components/Image.h"

void UDP_ObjectButtonWidget::Init(EObjectType ObjectType, UTexture2D* Thumbnail)
{
    Button->Init(Thumbnail);
    Type = ObjectType;
}

void UDP_ObjectButtonWidget::UpdateSelection(EObjectType ObjectType)
{
    FrameImage->SetVisibility(ObjectType == Type ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UDP_ObjectButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(FrameImage);
    check(Button);

    Button->OnClicked.AddUObject(this, &ThisClass::OnClickedHandler);
}

void UDP_ObjectButtonWidget::OnClickedHandler()
{
    OnClickedObjectButton.Broadcast(Type);
}
