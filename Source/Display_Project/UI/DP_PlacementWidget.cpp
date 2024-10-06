// Display_Project, all rights reserved.

#include "UI/DP_PlacementWidget.h"
#include "UI/DP_ObjectButtonWidget.h"
#include "UI/DP_AttributesListWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"

void UDP_PlacementWidget::CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    ButtonsBox->ClearChildren();
    AttributesSwitcher->ClearChildren();

    for (const auto& [ObjectType, ObjectData] : ObjectsMap)
    {
        static int32 WidgetID{0};
        ButtonsBox->AddChild(CreateButtonWidget(ObjectType, WidgetID));
        WidgetID = (WidgetID + 1) % ObjectsMap.Num();
        AttributesSwitcher->AddChild(CreateAttributesListWidget(ObjectType, ObjectData.Attributes));
    }
}

void UDP_PlacementWidget::HideAttributes()
{
    AttributesSwitcher->SetVisibility(ESlateVisibility::Collapsed);
}

void UDP_PlacementWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ButtonsBox);
    check(AttributesSwitcher);
    check(DestroyAllButton);

    DestroyAllButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedDestroyAllButtonHandler);
}

UDP_ObjectButtonWidget* UDP_PlacementWidget::CreateButtonWidget(EObjectType ObjectType, int32 WidgetID)
{
    auto* ObjectButtonWidget = CreateWidget<UDP_ObjectButtonWidget>(GetWorld(), ObjectButtonWidgetClass);
    check(ObjectButtonWidget);
    ObjectButtonWidget->Init(ObjectType, WidgetID);
    ObjectButtonWidget->OnClickedObjectButton.AddUObject(this, &ThisClass::OnClickedObjectButtonHandler);

    return ObjectButtonWidget;
}

UDP_AttributesListWidget* UDP_PlacementWidget::CreateAttributesListWidget(EObjectType ObjectType, const TArray<EAttributeType>& Attributes)
{
    auto* AttributesListWidget = CreateWidget<UDP_AttributesListWidget>(GetWorld(), AttributesListWidgetClass);
    check(AttributesListWidget);
    AttributesListWidget->Init(ObjectType, Attributes);
    AttributesListWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);

    return AttributesListWidget;
}

void UDP_PlacementWidget::ResetCurrentAttributesList()
{
    if (auto* AttributesListWidget = Cast<UDP_AttributesListWidget>(AttributesSwitcher->GetActiveWidget()))
    {
        AttributesListWidget->Reset();
    }
}

void UDP_PlacementWidget::OnClickedObjectButtonHandler(EObjectType ObjectType, int32 WidgetID)
{
    OnObjectTypeChanged.Broadcast(ObjectType);
    AttributesSwitcher->SetVisibility(ESlateVisibility::Visible);
    AttributesSwitcher->SetActiveWidgetIndex(WidgetID);
    ResetCurrentAttributesList();
}

void UDP_PlacementWidget::OnClickedDestroyAllButtonHandler()
{
    OnDestroyAll.Broadcast();
}

void UDP_PlacementWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}
