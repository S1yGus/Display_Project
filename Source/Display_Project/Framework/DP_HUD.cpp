// Display_Project, all rights reserved.

#include "Framework/DP_HUD.h"
#include "Framework/DP_GameModeBase.h"
#include "Blueprint/UserWidget.h"

void ADP_HUD::BeginPlay()
{
    Super::BeginPlay();

    check(ObjectPlacementWidgetClass);

    SetupWidgets();

    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ADP_GameModeBase>() : nullptr)
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void ADP_HUD::SetupWidgets()
{
    GameWidgets.Add(EGameState::ObjectPlacement, CreateWidget<UUserWidget>(GetWorld(), ObjectPlacementWidgetClass));

    for (const auto& [GameState, GameWidget] : GameWidgets)
    {
        if (GameWidget)
        {
            GameWidget->AddToViewport();
            GameWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void ADP_HUD::OnGameStateChanged(EGameState GameState)
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
