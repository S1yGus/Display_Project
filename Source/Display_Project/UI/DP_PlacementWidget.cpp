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

void UDP_PlacementWidget::HideAttributesList()
{
    AttributesSwitcher->SetVisibility(ESlateVisibility::Hidden);
    UpdateButtonsSelection(EObjectType::None);
}

void UDP_PlacementWidget::Copy(EObjectType ObjectType, const FAttributesMap& Attributes)
{
    UpdateSelection(ObjectType);
    UpdateCurrentAttributesList(Attributes);
}

void UDP_PlacementWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ButtonsBox);
    check(AttributesSwitcher);
    check(DestroyAllButton);

    DestroyAllButton->OnClicked.AddUObject(this, &ThisClass::OnClickedDestroyAllButtonHandler);

    HideAttributesList();
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

void UDP_PlacementWidget::UpdateCurrentAttributesList(const FAttributesMap& Attributes)
{
    if (auto* AttributesList = Cast<UDP_AttributesListWidget>(AttributesSwitcher->GetActiveWidget()))
    {
        AttributesList->Update(Attributes);
    }
}

void UDP_PlacementWidget::UpdateSelection(EObjectType ObjectType)
{
    if (TypeIDMap.Contains(ObjectType))
    {
        AttributesSwitcher->SetVisibility(ESlateVisibility::Visible);
        AttributesSwitcher->SetActiveWidgetIndex(TypeIDMap[ObjectType]);
        UpdateButtonsSelection(ObjectType);
        OnObjectTypeChanged.Broadcast(ObjectType);
    }
}

void UDP_PlacementWidget::UpdateButtonsSelection(EObjectType ObjectType)
{
    for (auto* Widget : ButtonsBox->GetAllChildren())
    {
        if (auto* Button = Cast<UDP_ObjectButtonWidget>(Widget))
        {
            Button->Select(ObjectType);
        }
    }
}

void UDP_PlacementWidget::OnClickedObjectButtonHandler(EObjectType ObjectType)
{
    UpdateSelection(ObjectType);
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
