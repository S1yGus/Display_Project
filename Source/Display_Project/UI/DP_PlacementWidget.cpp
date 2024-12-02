// Display_Project, all rights reserved.

#include "UI/DP_PlacementWidget.h"
#include "UI/DP_ObjectButtonWidget.h"
#include "UI/DP_AttributesListWidget.h"
#include "UI/DP_ButtonWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"

void UDP_PlacementWidget::CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    ButtonsBox->ClearChildren();
    AttributesSwitcher->ClearChildren();

    for (const auto& [ObjectType, ObjectData] : ObjectsMap)
    {
        ButtonsBox->AddChild(CreateButtonWidget(ObjectType, ObjectData.Thumbnail));
        AttributesSwitcher->AddChild(CreateAttributesListWidget(ObjectType, ObjectData.Attributes));

        static int32 ID{0};
        TypeIDMap.Add(ObjectType, ID);
        ID = (ID + 1) % ObjectsMap.Num();
    }
}

void UDP_PlacementWidget::DeselectPlacementObject()
{
    AttributesSwitcher->SetVisibility(ESlateVisibility::Hidden);
    UpdateObjectButtonsSelection(EObjectType::None);
}

void UDP_PlacementWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ButtonsBox);
    check(AttributesSwitcher);
    check(DestroyAllButton);

    DestroyAllButton->OnClicked.AddUObject(this, &ThisClass::OnClickedDestroyAllButtonHandler);

    DeselectPlacementObject();
}

UDP_ObjectButtonWidget* UDP_PlacementWidget::CreateButtonWidget(EObjectType ObjectType, UTexture2D* Thumbnail)
{
    auto* ObjectButtonWidget = CreateWidget<UDP_ObjectButtonWidget>(GetWorld(), ObjectButtonWidgetClass);
    check(ObjectButtonWidget);
    ObjectButtonWidget->Init(ObjectType, Thumbnail);
    ObjectButtonWidget->OnClickedObjectButton.AddUObject(this, &ThisClass::OnClickedObjectButtonHandler);

    return ObjectButtonWidget;
}

UDP_AttributesListWidget* UDP_PlacementWidget::CreateAttributesListWidget(EObjectType ObjectType, const TSet<EAttributeType>& Attributes)
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

void UDP_PlacementWidget::UpdateObjectButtonsSelection(EObjectType ObjectType)
{
    for (auto* Widget : ButtonsBox->GetAllChildren())
    {
        if (auto* ButtonWidget = Cast<UDP_ObjectButtonWidget>(Widget))
        {
            ButtonWidget->UpdateSelection(ObjectType);
        }
    }
}

void UDP_PlacementWidget::OnClickedObjectButtonHandler(EObjectType ObjectType)
{
    OnObjectTypeChanged.Broadcast(ObjectType);

    if (TypeIDMap.Contains(ObjectType))
    {
        AttributesSwitcher->SetVisibility(ESlateVisibility::Visible);
        AttributesSwitcher->SetActiveWidgetIndex(TypeIDMap[ObjectType]);
        ResetCurrentAttributesList();
    }

    UpdateObjectButtonsSelection(ObjectType);
}

void UDP_PlacementWidget::OnClickedDestroyAllButtonHandler()
{
    OnDestroyAll.Broadcast();
}

void UDP_PlacementWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}
