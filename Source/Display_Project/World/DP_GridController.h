// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "DP_CoreTypes.h"
#include "DP_GridController.generated.h"

class ADP_Grid;
class ADP_TextShifter;
class ADP_PlayerController;
class ADP_Player;
class ADP_HUD;
class ATargetPoint;

UCLASS()
class DISPLAY_PROJECT_API ADP_GridController : public AInfo
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Classes")
    TMap<EObjectType, FObjectData> ObjectsMap;

    UPROPERTY(EditAnywhere, Category = "Classes")
    TSubclassOf<ADP_Grid> GridClass;

    UPROPERTY(EditAnywhere, Category = "Welcome")
    TObjectPtr<ATargetPoint> WelcomePoint;

    UPROPERTY(EditAnywhere, Category = "Welcome")
    TArray<TObjectPtr<ADP_TextShifter>> WelcomeText;

    UPROPERTY(EditAnywhere, Category = "Welcome")
    float WelcomeDelay{2.0f};

    UPROPERTY(EditAnywhere, Category = "Warning")
    FText QuitWarning;

    UPROPERTY(EditAnywhere, Category = "Warning")
    FText DestroyAllWarning;

    virtual void BeginPlay() override;

private:
    EGameState CurrentGameState{EGameState::Standby};
    EGameState PrevGameState{EGameState::Standby};
    EObjectType CurrentObjectType{EObjectType::None};
    UPROPERTY()
    TObjectPtr<ADP_Grid> Grid;
    UPROPERTY()
    TObjectPtr<ADP_PlaceableActor> SelectedObject;
    FDeferredAction DeferredAction{nullptr};

    [[nodiscard]] FORCEINLINE ADP_PlayerController* GetPlayerController() const;
    [[nodiscard]] FORCEINLINE ADP_Player* GetPlayer() const;
    [[nodiscard]] FORCEINLINE ADP_HUD* GetHUD() const;
    FORCEINLINE void UpdatePlayerLocation(const FVector& Location);
    FORCEINLINE void ShowWarning(const FText& WarningText, FDeferredAction&& Action);

    FORCEINLINE void SetupPlayerController();
    FORCEINLINE void SetupHUD();
    FORCEINLINE void SpawnGrid();
    FORCEINLINE void InitWelcomeState();

    void SetGameState(EGameState NewGameState);
    void SetGameState_Internal(EGameState NewGameState);
    void SetCurrentObjectType(EObjectType NewObjectType);
    void SetCurrentObjectType_Internal(EObjectType NewObjectType);

    void OnSwitchToGameHandler();
    void OnUpdatePreviewLocationHandler(AActor* ReferenceActor);
    void OnSpawnCurrentObjectHandler();
    void OnObjectSpawnCompletedHandler();
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
    void OnSelectHandler(AActor* SelectedActor);
    void OnDestroySelectedHandler();
    void OnDestroyAllHandler();
    void OnQuitHandler();
    void OnToggleScreenModeHandler();
    void OnShowOptionsHandler();
    void OnStopShowingOptionsHandler();
    void OnVideoQualityChangedHandler(EVideoQuality VideoQuality);
    void OnRotationSpeedChangedHandler(float RotationSpeedNormalized);
    void OnSoundVolumeChangedHandler(float SoundVolume);
    void OnShowHelpHandler();
    void OnWarningResponseHandler(bool bCondition);
    void OnInspectHandler();
    void OnInspectCompletedHandler();
};
