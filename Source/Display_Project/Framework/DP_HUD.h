// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DP_CoreTypes.h"
#include "DP_HUD.generated.h"

UCLASS()
class DISPLAY_PROJECT_API ADP_HUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PlacementWidgetClass;

    virtual void BeginPlay() override;
    void CreateWidgets();

private:
    UPROPERTY()
    TMap<EGameState, TObjectPtr<UUserWidget>> GameWidgets;
    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

    void OnGameStateChanged(EGameState GameState);
};
