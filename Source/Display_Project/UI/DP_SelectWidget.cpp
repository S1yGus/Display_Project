// Display_Project, all rights reserved.

#include "UI/DP_SelectWidget.h"
#include "UI/DP_ObjectInfoWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UDP_SelectWidget::CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    AttributesSwitcher->ClearChildren();

    for (const auto& [ObjectType, ObjectData] : ObjectsMap)
    {
        auto* ObjectInfoWidget = CreateWidget<UDP_ObjectInfoWidget>(GetWorld(), ObjectInfoWidgetClass);
        check(ObjectInfoWidget);
        ObjectInfoWidget->Init(ObjectType, ObjectData.Attributes);
        ObjectInfoWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
        AttributesSwitcher->AddChild(ObjectInfoWidget);

        static int32 ID{0};
        TypeIDMap.Add(ObjectType, ID);
        ID = (ID + 1) % ObjectsMap.Num();
    }
}

void UDP_SelectWidget::Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes)
{
    if (TypeIDMap.Contains(ObjectType))
    {
        AttributesSwitcher->SetActiveWidgetIndex(TypeIDMap[ObjectType]);
        if (auto* ObjectInfoWidget = Cast<UDP_ObjectInfoWidget>(AttributesSwitcher->GetActiveWidget()))
        {
            ObjectInfoWidget->Select(ObjectName, Attributes);
        }
    }
}

void UDP_SelectWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(InspectButton);
    check(DestroyButton);
    check(AttributesSwitcher);

    InspectButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedInspectButtonHandler);
    DestroyButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedDestroyButtonHandler);
}

void UDP_SelectWidget::OnClickedInspectButtonHandler()
{
    // TODO
}

void UDP_SelectWidget::OnClickedDestroyButtonHandler()
{
    OnDestroySelected.Broadcast();
}

void UDP_SelectWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}
