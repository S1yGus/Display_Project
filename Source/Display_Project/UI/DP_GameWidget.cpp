// Display_Project, all rights reserved.

#include "UI/DP_GameWidget.h"
#include "UI/DP_PlacementWidget.h"
#include "UI/DP_SelectWidget.h"
#include "UI/DP_ControlPanelWidget.h"
#include "Components/WidgetSwitcher.h"

static constexpr int32 PlacementWidgetIndex{0};
static constexpr int32 SelectWidgetIndex{1};

void UDP_GameWidget::CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    PlacementWidget->CreateWidgetsForObjects(ObjectsMap);
    SelectWidget->CreateWidgetsForObjects(ObjectsMap);
}

void UDP_GameWidget::SwitchCurrentWidget(EGameState GameState)
{
    if (GameState == EGameState::Select)
    {
        WidgetSwitcher->SetActiveWidgetIndex(SelectWidgetIndex);
    }
    else if (GameState == EGameState::Placement || GameState == EGameState::Interact)
    {
        WidgetSwitcher->SetActiveWidgetIndex(PlacementWidgetIndex);
    }
}

void UDP_GameWidget::Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes)
{
    SelectWidget->Select(ObjectType, ObjectName, Attributes);
}

void UDP_GameWidget::DeselectPlacementObject()
{
    PlacementWidget->DeselectPlacementObject();
}

void UDP_GameWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(WidgetSwitcher);
    check(PlacementWidget);
    check(SelectWidget);
    check(ControlPanelWidget);

    PlacementWidget->OnObjectTypeChanged.AddUObject(this, &ThisClass::OnObjectTypeChangedHandler);
    PlacementWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
    PlacementWidget->OnDestroyAll.AddUObject(this, &ThisClass::OnDestroyAllHandler);

    SelectWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
    SelectWidget->OnDestroySelected.AddUObject(this, &ThisClass::OnDestroySelectedHandler);

    ControlPanelWidget->OnQuit.AddUObject(this, &ThisClass::OnQuitHandler);
    ControlPanelWidget->OnToggleScreenMode.AddUObject(this, &ThisClass::OnToggleScreenModeHandler);
    ControlPanelWidget->OnShowHelp.AddUObject(this, &ThisClass::OnShowHelpHandler);
}

void UDP_GameWidget::OnObjectTypeChangedHandler(EObjectType ObjectType)
{
    OnObjectTypeChanged.Broadcast(ObjectType);
}

void UDP_GameWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}

void UDP_GameWidget::OnDestroyAllHandler()
{
    OnDestroyAll.Broadcast();
}

void UDP_GameWidget::OnDestroySelectedHandler()
{
    OnDestroySelected.Broadcast();
}

void UDP_GameWidget::OnQuitHandler()
{
    OnQuit.Broadcast();
}

void UDP_GameWidget::OnToggleScreenModeHandler()
{
    OnToggleScreenMode.Broadcast();
}

void UDP_GameWidget::OnShowHelpHandler()
{
    OnShowHelp.Broadcast();
}