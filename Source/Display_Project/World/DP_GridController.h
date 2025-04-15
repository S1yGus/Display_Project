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
class ADP_GameModeBase;
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

    UPROPERTY(EditAnywhere, Category = "Warning")
    FText LoadWarning;

    UPROPERTY(EditAnywhere, Category = "Warning")
    FText DeleteSaveWarning;

    virtual void BeginPlay() override;

private:
    EGameState CurrentGameState{EGameState::Standby};
    TArray<EGameState> PrevGameStates;
    EObjectType CurrentObjectType{EObjectType::None};
    UPROPERTY()
    TObjectPtr<ADP_Grid> Grid;
    UPROPERTY()
    TObjectPtr<ADP_PlaceableActor> SelectedObject;
    TDeferredAction DeferredAction{nullptr};

    [[nodiscard]] FORCEINLINE ADP_PlayerController* GetPlayerController() const;
    [[nodiscard]] FORCEINLINE ADP_Player* GetPlayer() const;
    [[nodiscard]] FORCEINLINE ADP_HUD* GetHUD() const;
    [[nodiscard]] FORCEINLINE ADP_GameModeBase* GetGameMode() const;

    FORCEINLINE void UpdatePlayerLocation(EGameState GameState);
    FORCEINLINE void ShowWarning(const FText& WarningText, TDeferredAction&& Action);
    FORCEINLINE void UpdateUISaveRecords();

    FORCEINLINE void SetupGameMode();
    FORCEINLINE void SetupPlayerController();
    FORCEINLINE void SetupHUD();
    FORCEINLINE void SpawnGrid();
    FORCEINLINE void InitWelcomeState();
    FORCEINLINE void SpawnObjectsFromSave(const TArray<FObjectSaveData>& SaveObjectData);
    FORCEINLINE void DestroySelectedObject();
    FORCEINLINE void DeselectPlacementObject();
    FORCEINLINE void HandleSelectGameState();

    void UpdatePrevGameState(EGameState GameState);
    void SetGameState(EGameState NewGameState);
    void SetPrevGameState();
    void SetGameState_Internal(EGameState NewGameState);
    void SetCurrentObjectType(EObjectType NewObjectType);
    void SetCurrentObjectType_Internal(EObjectType NewObjectType);

    void OnSwitchToGameHandler();
    void OnUpdatePreviewLocationHandler(AActor* ReferenceActor);
    void OnSpawnCurrentObjectHandler();
    void OnObjectSpawnCompletedHandler(AActor* Actor, bool bMoved);
    void OnAttributeChangedHandler(EAttributeType AttributeType, TAttributeData AttributeData);
    void OnSelectHandler(AActor* SelectedActor);
    void OnDeselectPlacementObjectHandler();
    void OnDestroySelectedHandler();
    void OnDestroyAllHandler();
    void OnQuitHandler();
    void OnToggleScreenModeHandler();
    void OnShowOptionsHandler();
    void OnBackHandler();
    void OnVideoQualityChangedHandler(EVideoQuality VideoQuality);
    void OnRotationSpeedChangedHandler(float RotationSpeedNormalized);
    void OnSoundVolumeChangedHandler(float SoundVolume);
    void OnShowSaveAndLoadHandler();
    void OnSaveHandler(const FText& SaveName);
    void OnLoadHandler(const FGuid& Guid);
    void OnDeleteSaveHandler(const FGuid& Guid);
    void OnShowHelpHandler();
    void OnWarningResponseHandler(bool bCondition);
    void OnCopyHandler();
    void OnMoveHandler();
    void OnInspectHandler();
    void OnInspectCompletedHandler();
    void OnAssetsPreloadCompletedHandler();
};
