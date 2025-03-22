// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DP_CoreTypes.h"
#include "DP_HUD.generated.h"

class UDP_BaseAnimatedWidget;
class UDP_GameWidget;
class UDP_SaveAndLoadWidget;

UCLASS()
class DISPLAY_PROJECT_API ADP_HUD : public AHUD
{
    GENERATED_BODY()

public:
    FOnObjectTypeChangedSignature OnObjectTypeChanged;
    FOnAttributeChangedSignature OnAttributeChanged;
    FOnDestroySelectedSignature OnDestroySelected;
    FOnDestroyAllSignature OnDestroyAll;
    FOnQuitSignature OnQuit;
    FOnToggleScreenModeSignature OnToggleScreenMode;
    FOnShowOptionsSignature OnShowOptions;
    FOnBackSignature OnBack;
    FOnVideoQualityChangedSignature OnVideoQualityChanged;
    FOnRotationSpeedChangedSignature OnRotationSpeedChanged;
    FOnSoundVolumeChangedSignature OnSoundVolumeChanged;
    FOnShowSaveAndLoadSignature OnShowSaveAndLoad;
    FOnSaveSignature OnSave;
    FOnLoadSignature OnLoad;
    FOnDeleteSaveSignature OnDeleteSave;
    FOnShowHelpSignature OnShowHelp;
    FOnWarningResponseSignature OnWarningResponse;
    FOnInspectSignature OnInspect;
    FOnInspectCompletedSignature OnInspectCompleted;

    void CreateWidgets(const TMap<EObjectType, FObjectData>& ObjectsMap, const FVideoQualityOptionsData& VideoQualityOptionsData, float RotationSpeedNormalized,
                       float SoundVolume);
    void ChangeCurrentWidget(EGameState GameState);
    void UpdateSaves(const TArray<FSaveRecordMetaData>& SaveRecordsMetaData);
    void DeselectPlacementObject();
    void Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes);
    bool ShowWarning(const FText& WarningText);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> PreloadWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> WelcomeWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> GameWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> InspectWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> OptionsWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> SaveAndLoadWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EWidgetType, TObjectPtr<UDP_BaseAnimatedWidget>> Widgets;
    UPROPERTY()
    TObjectPtr<UDP_BaseAnimatedWidget> CurrentWidget;
    EWidgetType CurrentWidgetType{EWidgetType::Welcome};

    [[nodiscard]] FORCEINLINE UDP_GameWidget* GetGameWidget() const;
    [[nodiscard]] FORCEINLINE UDP_SaveAndLoadWidget* GetSaveAndLoadWidget() const;

    FORCEINLINE void CreatePreloadWidget();
    FORCEINLINE void CreateWelcomeWidget();
    FORCEINLINE void CreateGameWidget(const TMap<EObjectType, FObjectData>& ObjectsMap);
    FORCEINLINE void CreateInspectWidget();
    FORCEINLINE void CreateOptionsWidget(const FVideoQualityOptionsData& VideoQualityOptionsData, float RotationSpeedNormalized, float SoundVolume);
    FORCEINLINE void CreateSaveAndLoadWidget();

    FORCEINLINE void SetCurrentWidget();
    FORCEINLINE void HandleGameWidget(EGameState GameState);

    void OnObjectTypeChangedHandler(EObjectType ObjectType);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
    void OnDestroyAllHandler();
    void OnDestroySelectedHandler();
    void OnFadeoutAnimationFinishedHandler();
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
    void OnInspectHandler();
    void OnInspectCompletedHandler();
};
