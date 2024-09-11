// Display_Project, all rights reserved.

#include "UI/DP_PlacementWidget.h"
#include "UI/DP_ObjectButtonWidget.h"
#include "UI/DP_AttributesListWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Framework/DP_GameModeBase.h"

void UDP_PlacementWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ButtonsBox);
    check(AttributesSwitcher);
    check(FreeAllButton);

    CreateWidgetsForObjects();

    FreeAllButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedFreeButton);
    if (auto* GameMode = GetGameMode())
    {
        GameMode->OnObjectTypeChanged.AddUObject(this, &ThisClass::OnObjectTypeChanged);
    }
}

ADP_GameModeBase* UDP_PlacementWidget::GetGameMode() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ADP_GameModeBase>() : nullptr;
}

void UDP_PlacementWidget::CreateWidgetsForObjects()
{
    ButtonsBox->ClearChildren();
    AttributesSwitcher->ClearChildren();

    if (auto* GameMode = GetGameMode())
    {
        const auto& ObjectsMap = GameMode->GetObjectsMap();
        for (const auto& [ObjectType, ObjectData] : ObjectsMap)
        {
            static int32 WidgetID{0};
            ButtonsBox->AddChild(CreateButtonWidget(ObjectType, ObjectData.Name, WidgetID));
            WidgetID = (WidgetID + 1) % ObjectsMap.Num();
            AttributesSwitcher->AddChild(CreateAttributesListWidget(ObjectType, ObjectData.AttributesMap));
        }
    }
}

UDP_ObjectButtonWidget* UDP_PlacementWidget::CreateButtonWidget(EObjectType ObjectType, const FText& ObjectName, int32 WidgetID)
{
    auto* ObjectButtonWidget = CreateWidget<UDP_ObjectButtonWidget>(GetWorld(), ObjectButtonWidgetClass);
    check(ObjectButtonWidget);
    ObjectButtonWidget->Init(ObjectType, ObjectName, WidgetID);
    ObjectButtonWidget->OnClickedObjectButton.AddUObject(this, &ThisClass::OnClickedObjectButton);

    return ObjectButtonWidget;
}

UDP_AttributesListWidget* UDP_PlacementWidget::CreateAttributesListWidget(EObjectType ObjectType, const FAttributesMap& Attributes)
{
    auto* AttributesListWidget = CreateWidget<UDP_AttributesListWidget>(GetWorld(), AttributesListWidgetClass);
    check(AttributesListWidget);
    AttributesListWidget->Init(ObjectType, Attributes);
    AttributesListWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);

    return AttributesListWidget;
}

void UDP_PlacementWidget::ResetCurrentObjectAttributes()
{
    if (auto* AttributesListWidget = Cast<UDP_AttributesListWidget>(AttributesSwitcher->GetActiveWidget()))
    {
        AttributesListWidget->Reset();
    }
}

void UDP_PlacementWidget::OnClickedObjectButton(EObjectType ObjectType, int32 WidgetID)
{
    if (auto* GameMode = GetGameMode())
    {
        GameMode->SetCurrentObjectType(ObjectType);
        GameMode->SetGameState(EGameState::Placement);

        AttributesSwitcher->SetVisibility(ESlateVisibility::Visible);
        AttributesSwitcher->SetActiveWidgetIndex(WidgetID);
        ResetCurrentObjectAttributes();
    }
}

void UDP_PlacementWidget::OnClickedFreeButton()
{
    if (auto* GameMode = GetGameMode())
    {
        GameMode->SetGameState(EGameState::Standby);
    }
}

void UDP_PlacementWidget::OnObjectTypeChanged(EObjectType ObjectType)
{
    if (ObjectType == EObjectType::None)
    {
        AttributesSwitcher->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UDP_PlacementWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (auto* GameMode = GetGameMode())
    {
        GameMode->AddCurrentObjectAttribute(AttributeType, AttributeData);
    }
}
