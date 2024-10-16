// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DP_CoreTypes.h"
#include "DP_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class DISPLAY_PROJECT_API ADP_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    FOnUpdatePreviewLocationSignature OnUpdatePreviewLocation;
    FOnRequestObjectSpawnSignature OnRequestObjectSpawn;
    FOnObjectSelectedSignature OnObjectSelected;
    FOnWelcomeScreenCompletedSignature OnWelcomeScreenCompleted;

    ADP_PlayerController();

    void UpdateGameState(EGameState NewGameState);
    void UpdatePlayerLocation(const FVector& NewLocation);

    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ClickAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> SelectAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> AnyKeyAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> WelcomeInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    float UpdatePlayerLocationSpeed{6.0f};

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    EGameState CurrentGameState{EGameState::Standby};
    FVector TargetPlayerLocation{FVector::ZeroVector};
    FTimerHandle UpdatePlayerLocationTimerHandle;

    void UpdateInputMappingContext();

    void ObjectPlacementClick();
    void InteractClick();

    void OnClickHandler();
    void OnSelectHandler();
    void OnPressAnyKeyHandler();
    void OnUpdatePlayerLocationHandler();
};
