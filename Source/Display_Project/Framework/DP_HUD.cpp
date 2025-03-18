// Display_Project, all rights reserved.

#include "Framework/DP_HUD.h"
#include "UI/DP_BaseAnimatedWidget.h"
#include "UI/DP_GameWidget.h"
#include "UI/DP_InspectWidget.h"
#include "UI/DP_OptionsWidget.h"
#include "UI/DP_SaveAndLoadWidget.h"
#include "UI/DP_AnimatedWidgetWithWarning.h"
#include "DP_Utils.h"

void ADP_HUD::CreateWidgets(const TMap<EObjectType, FObjectData>& ObjectsMap,           //
                            const FVideoQualityOptionsData& VideoQualityOptionsData,    //
                            float RotationSpeedNormalized,                              //
                            float SoundVolume)
{
    CreateWelcomeWidget();
    CreateGameWidget(ObjectsMap);
    CreateInspectWidget();
    CreateOptionsWidget(VideoQualityOptionsData, RotationSpeedNormalized, SoundVolume);
    CreateSaveAndLoadWidget();

    for (auto& [Type, Widget] : Widgets)
    {
        Widget->AddToViewport();
        Widget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ADP_HUD::ChangeCurrentWidget(EGameState GameState)
{
    CurrentWidgetType = UI::GameStateToWidgetType(GameState);

    if (Widgets.Contains(CurrentWidgetType))
    {
        HandleGameWidget(GameState);

        if (CurrentWidget)
        {
            if (CurrentWidget != Widgets[CurrentWidgetType])
            {
                CurrentWidget->ShowFadeoutAnimation();
            }
        }
        else
        {
            SetCurrentWidget();
        }
    }
}

void ADP_HUD::UpdateSaves(const TArray<FSaveRecordMetaData>& SaveRecordsMetaData)
{
    if (auto* SaveAndLoadWidget = GetSaveAndLoadWidget())
    {
        SaveAndLoadWidget->UpdateSaves(SaveRecordsMetaData);
    }
}

void ADP_HUD::DeselectPlacementObject()
{
    if (auto* GameWidget = GetGameWidget())
    {
        GameWidget->DeselectPlacementObject();
    }
}

void ADP_HUD::Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes)
{
    if (auto* GameWidget = GetGameWidget())
    {
        GameWidget->Select(ObjectType, ObjectName, Attributes);
    }
}

bool ADP_HUD::ShowWarning(const FText& WarningText)
{
    if (auto* WidgetWithWarning = Cast<UDP_AnimatedWidgetWithWarning>(CurrentWidget))
    {
        WidgetWithWarning->ShowWarning(WarningText);
        return true;
    }

    return false;
}

void ADP_HUD::BeginPlay()
{
    Super::BeginPlay();

    check(PreloadWidgetClass);
    check(WelcomeWidgetClass);
    check(GameWidgetClass);
    check(InspectWidgetClass);
    check(OptionsWidgetClass);
    check(SaveAndLoadWidgetClass);
}

UDP_GameWidget* ADP_HUD::GetGameWidget() const
{
    if (Widgets.Contains(EWidgetType::Game))
    {
        return Cast<UDP_GameWidget>(Widgets[EWidgetType::Game]);
    }

    return nullptr;
}

UDP_SaveAndLoadWidget* ADP_HUD::GetSaveAndLoadWidget() const
{
    if (Widgets.Contains(EWidgetType::SaveAndLoad))
    {
        return Cast<UDP_SaveAndLoadWidget>(Widgets[EWidgetType::SaveAndLoad]);
    }

    return nullptr;
}

void ADP_HUD::CreatePreloadWidget()
{
    auto* PreloadWidget = CreateWidget<UDP_BaseAnimatedWidget>(GetWorld(), PreloadWidgetClass);
    check(PreloadWidget);
    PreloadWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnFadeoutAnimationFinishedHandler);
    Widgets.Add(EWidgetType::Preload, PreloadWidget);
}

void ADP_HUD::CreateWelcomeWidget()
{
    auto* WelcomeWidget = CreateWidget<UDP_BaseAnimatedWidget>(GetWorld(), WelcomeWidgetClass);
    check(WelcomeWidget);
    WelcomeWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnFadeoutAnimationFinishedHandler);
    Widgets.Add(EWidgetType::Welcome, WelcomeWidget);
}

void ADP_HUD::CreateGameWidget(const TMap<EObjectType, FObjectData>& ObjectsMap)
{
    auto* GameWidget = CreateWidget<UDP_GameWidget>(GetWorld(), GameWidgetClass);
    check(GameWidget);
    GameWidget->OnObjectTypeChanged.AddUObject(this, &ThisClass::OnObjectTypeChangedHandler);
    GameWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
    GameWidget->OnDestroyAll.AddUObject(this, &ThisClass::OnDestroyAllHandler);
    GameWidget->OnDestroySelected.AddUObject(this, &ThisClass::OnDestroySelectedHandler);
    GameWidget->OnQuit.AddUObject(this, &ThisClass::OnQuitHandler);
    GameWidget->OnToggleScreenMode.AddUObject(this, &ThisClass::OnToggleScreenModeHandler);
    GameWidget->OnShowOptions.AddUObject(this, &ThisClass::OnShowOptionsHandler);
    GameWidget->OnShowSaveAndLoad.AddUObject(this, &ThisClass::OnShowSaveAndLoadHandler);
    GameWidget->OnShowHelp.AddUObject(this, &ThisClass::OnShowHelpHandler);
    GameWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnFadeoutAnimationFinishedHandler);
    GameWidget->OnWarningResponse.AddUObject(this, &ThisClass::OnWarningResponseHandler);
    GameWidget->OnInspect.AddUObject(this, &ThisClass::OnInspectHandler);
    GameWidget->CreateWidgetsForObjects(ObjectsMap);
    Widgets.Add(EWidgetType::Game, GameWidget);
}

void ADP_HUD::CreateInspectWidget()
{
    auto* InspectWidget = CreateWidget<UDP_InspectWidget>(GetWorld(), InspectWidgetClass);
    check(InspectWidget);
    InspectWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnFadeoutAnimationFinishedHandler);
    InspectWidget->OnInspectCompleted.AddUObject(this, &ThisClass::OnInspectCompletedHandler);
    Widgets.Add(EWidgetType::Inspect, InspectWidget);
}

void ADP_HUD::CreateOptionsWidget(const FVideoQualityOptionsData& VideoQualityOptionsData, float RotationSpeedNormalized, float SoundVolume)
{
    auto* OptionsWidget = CreateWidget<UDP_OptionsWidget>(GetWorld(), OptionsWidgetClass);
    check(OptionsWidget);
    OptionsWidget->OnBack.AddUObject(this, &ThisClass::OnBackHandler);
    OptionsWidget->OnVideoQualityChanged.AddUObject(this, &ThisClass::OnVideoQualityChangedHandler);
    OptionsWidget->OnRotationSpeedChanged.AddUObject(this, &ThisClass::OnRotationSpeedChangedHandler);
    OptionsWidget->OnSoundVolumeChanged.AddUObject(this, &ThisClass::OnSoundVolumeChangedHandler);
    OptionsWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnFadeoutAnimationFinishedHandler);
    OptionsWidget->Init(VideoQualityOptionsData, RotationSpeedNormalized, SoundVolume);
    Widgets.Add(EWidgetType::Options, OptionsWidget);
}

void ADP_HUD::CreateSaveAndLoadWidget()
{
    auto* SaveAndLoadWidget = CreateWidget<UDP_SaveAndLoadWidget>(GetWorld(), SaveAndLoadWidgetClass);
    check(SaveAndLoadWidget);
    SaveAndLoadWidget->OnBack.AddUObject(this, &ThisClass::OnBackHandler);
    SaveAndLoadWidget->OnSave.AddUObject(this, &ThisClass::OnSaveHandler);
    SaveAndLoadWidget->OnLoad.AddUObject(this, &ThisClass::OnLoadHandler);
    SaveAndLoadWidget->OnDeleteSave.AddUObject(this, &ThisClass::OnDeleteSaveHandler);
    SaveAndLoadWidget->OnWarningResponse.AddUObject(this, &ThisClass::OnWarningResponseHandler);
    SaveAndLoadWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnFadeoutAnimationFinishedHandler);
    Widgets.Add(EWidgetType::SaveAndLoad, SaveAndLoadWidget);
}

void ADP_HUD::SetCurrentWidget()
{
    CurrentWidget = Widgets[CurrentWidgetType];
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->ShowStartupAnimation();
    }
}

void ADP_HUD::HandleGameWidget(EGameState GameState)
{
    if (CurrentWidgetType == EWidgetType::Game)
    {
        if (auto* GameWidget = GetGameWidget())
        {
            GameWidget->SwitchCurrentWidget(GameState);
        }
    }
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

void ADP_HUD::OnFadeoutAnimationFinishedHandler()
{
    CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    SetCurrentWidget();
}

void ADP_HUD::OnQuitHandler()
{
    OnQuit.Broadcast();
}

void ADP_HUD::OnToggleScreenModeHandler()
{
    OnToggleScreenMode.Broadcast();
}

void ADP_HUD::OnShowOptionsHandler()
{
    OnShowOptions.Broadcast();
}

void ADP_HUD::OnBackHandler()
{
    OnBack.Broadcast();
}

void ADP_HUD::OnVideoQualityChangedHandler(EVideoQuality VideoQuality)
{
    OnVideoQualityChanged.Broadcast(VideoQuality);
}

void ADP_HUD::OnRotationSpeedChangedHandler(float RotationSpeedNormalized)
{
    OnRotationSpeedChanged.Broadcast(RotationSpeedNormalized);
}

void ADP_HUD::OnSoundVolumeChangedHandler(float SoundVolume)
{
    OnSoundVolumeChanged.Broadcast(SoundVolume);
}

void ADP_HUD::OnShowSaveAndLoadHandler()
{
    OnShowSaveAndLoad.Broadcast();
}

void ADP_HUD::OnSaveHandler(const FText& SaveName)
{
    OnSave.Broadcast(SaveName);
}

void ADP_HUD::OnLoadHandler(const FGuid& Guid)
{
    OnLoad.Broadcast(Guid);
}

void ADP_HUD::OnDeleteSaveHandler(const FGuid& Guid)
{
    OnDeleteSave.Broadcast(Guid);
}

void ADP_HUD::OnShowHelpHandler()
{
    OnShowHelp.Broadcast();
}

void ADP_HUD::OnWarningResponseHandler(bool bCondition)
{
    OnWarningResponse.Broadcast(bCondition);
}

void ADP_HUD::OnInspectHandler()
{
    OnInspect.Broadcast();
}

void ADP_HUD::OnInspectCompletedHandler()
{
    OnInspectCompleted.Broadcast();
}
