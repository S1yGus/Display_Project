// Display_Project, all rights reserved.

#include "UI/DP_SelectWidget.h"
#include "UI/DP_ObjectInfoWidget.h"
#include "UI/DP_ButtonWidget.h"
#include "Components/WidgetSwitcher.h"

void UDP_SelectWidget::CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    AttributesSwitcher->ClearChildren();

    for (const auto& [ObjectType, ObjectData] : ObjectsMap)
    {
        auto* ObjectInfoWidget = CreateWidget<UDP_ObjectInfoWidget>(GetWorld(), ObjectInfoWidgetClass);
        check(ObjectInfoWidget);
        ObjectInfoWidget->Init(ObjectType, ObjectData.Thumbnail, ObjectData.Attributes);
        ObjectInfoWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
        AttributesSwitcher->AddChild(ObjectInfoWidget);

        static int32 ID{0};
        TypeIDMap.Add(ObjectType, ID);
        ID = (ID + 1) % ObjectsMap.Num();
    }
}

void UDP_SelectWidget::Select(EObjectType ObjectType, const TAttributesMap& Attributes)
{
    if (TypeIDMap.Contains(ObjectType))
    {
        AttributesSwitcher->SetActiveWidgetIndex(TypeIDMap[ObjectType]);
        if (auto* ObjectInfoWidget = Cast<UDP_ObjectInfoWidget>(AttributesSwitcher->GetActiveWidget()))
        {
            ObjectInfoWidget->UpdateAttributes(Attributes);
        }
    }
}

void UDP_SelectWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(CopyButton);
    check(MoveButton);
    check(InspectButton);
    check(DestroyButton);
    check(AttributesSwitcher);

    CopyButton->OnClicked.AddUObject(this, &ThisClass::OnClickedCopyButtonHandler);
    MoveButton->OnClicked.AddUObject(this, &ThisClass::OnClickedMoveButtonHandler);
    InspectButton->OnClicked.AddUObject(this, &ThisClass::OnClickedInspectButtonHandler);
    DestroyButton->OnClicked.AddUObject(this, &ThisClass::OnClickedDestroyButtonHandler);
}

void UDP_SelectWidget::OnClickedCopyButtonHandler()
{
    OnCopy.Broadcast();
}

void UDP_SelectWidget::OnClickedMoveButtonHandler()
{
    OnMove.Broadcast();
}

void UDP_SelectWidget::OnClickedInspectButtonHandler()
{
    OnInspect.Broadcast();
}

void UDP_SelectWidget::OnClickedDestroyButtonHandler()
{
    OnDestroySelected.Broadcast();
}

void UDP_SelectWidget::OnAttributeChangedHandler(EAttributeType AttributeType, TAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}
