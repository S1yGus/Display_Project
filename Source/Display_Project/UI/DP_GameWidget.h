// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_AnimatedWidgetWithWarning.h"
#include "DP_CoreTypes.h"
#include "DP_GameWidget.generated.h"

class UWidgetSwitcher;
class UDP_PlacementWidget;
class UDP_SelectWidget;
class UDP_ControlPanelWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_GameWidget : public UDP_AnimatedWidgetWithWarning
{
    GENERATED_BODY()

public:
    FOnObjectTypeChangedSignature OnObjectTypeChanged;
    FOnAttributeChangedSignature OnAttributeChanged;
    FOnDestroyAllSignature OnDestroyAll;
    FOnDestroySelectedSignature OnDestroySelected;
    FOnQuitSignature OnQuit;
    FOnToggleScreenModeSignature OnToggleScreenMode;
    FOnShowHelpSignature OnShowHelp;

    void CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap);
    void SwitchCurrentWidget(EGameState GameState);
    void Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes);
    void DeselectPlacementObject();

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_PlacementWidget> PlacementWidget;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_SelectWidget> SelectWidget;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ControlPanelWidget> ControlPanelWidget;

    virtual void NativeOnInitialized() override;

private:
    void OnObjectTypeChangedHandler(EObjectType ObjectType);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
    void OnDestroyAllHandler();
    void OnDestroySelectedHandler();
    void OnQuitHandler();
    void OnToggleScreenModeHandler();
    void OnShowHelpHandler();
};
