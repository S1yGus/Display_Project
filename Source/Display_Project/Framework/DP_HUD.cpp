// Display_Project, all rights reserved.

#include "Framework/DP_HUD.h"
#include "UI/DP_PlacementWidget.h"
#include "UI/DP_SelectWidget.h"

void ADP_HUD::CreateWidgets(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    auto* WelcomeWidget = CreateWidget<UUserWidget>(GetWorld(), WelcomeWidgetClasses);
    check(WelcomeWidget);
    GameWidgets.Add(EGameState::Welcome, WelcomeWidget);

    auto* PlacementWidget = CreateWidget<UDP_PlacementWidget>(GetWorld(), PlacementWidgetClasses);
    check(PlacementWidget);
    PlacementWidget->OnObjectTypeChanged.AddUObject(this, &ThisClass::OnObjectTypeChangedHandler);
    PlacementWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
    PlacementWidget->OnDestroyAll.AddUObject(this, &ThisClass::OnDestroyAllHandler);
    PlacementWidget->CreateWidgetsForObjects(ObjectsMap);
    GameWidgets.Add(EGameState::Interact, PlacementWidget);

    auto* SelectWidget = CreateWidget<UDP_SelectWidget>(GetWorld(), SelectWidgetClasses);
    check(SelectWidget);
    SelectWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
    SelectWidget->OnDestroySelected.AddUObject(this, &ThisClass::OnDestroySelectedHandler);
    SelectWidget->CreateWidgetsForObjects(ObjectsMap);
    GameWidgets.Add(EGameState::Select, SelectWidget);

    for (auto& [State, Widget] : GameWidgets)
    {
        Widget->AddToViewport();
        Widget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ADP_HUD::ChangeCurrentWidget(EGameState GameState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GameWidgets.Contains(GameState))
    {
        CurrentWidget = GameWidgets[GameState];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void ADP_HUD::HideWidgetAttributes()
{
    if (auto* PlacementWidget = Cast<UDP_PlacementWidget>(GameWidgets[EGameState::Interact]))
    {
        PlacementWidget->HideAttributes();
    }
}

void ADP_HUD::Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes)
{
    if (auto* SelectWidget = Cast<UDP_SelectWidget>(GameWidgets[EGameState::Select]))
    {
        SelectWidget->Select(ObjectType, ObjectName, Attributes);
    }
}

void ADP_HUD::BeginPlay()
{
    Super::BeginPlay();

    check(WelcomeWidgetClasses);
    check(PlacementWidgetClasses);
    check(SelectWidgetClasses);
}

void ADP_HUD::OnObjectTypeChangedHandler(EObjectType ObjectType)
{
    OnObjectTypeChanged.Broadcast(ObjectType);
}

void ADP_HUD::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}

void ADP_HUD::OnDestroyAllHandler()
{
    OnDestroyAll.Broadcast();
}

void ADP_HUD::OnDestroySelectedHandler()
{
    OnDestroySelected.Broadcast();
}
