// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DP_CoreTypes.h"
#include "DP_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class ADP_GameModeBase;
class ADP_Grid;

UCLASS()
class DISPLAY_PROJECT_API ADP_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADP_PlayerController();

    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ClickAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> SelectAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    EGameState CurrentGameState{EGameState::Standby};

    [[nodiscard]] ADP_GameModeBase* GetGameMode();

    void ObjectPlacementClick();
    void InteractClick();

    void OnClick();
    void OnSelect();
    void OnGameStateChanged(EGameState NewGameState);
};
