// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DP_CoreTypes.h"
#include "DP_HUD.generated.h"

class UDP_BaseAnimatedWidget;
class UDP_GameWidget;

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
    FOnShowHelpSignature OnShowHelp;
    FOnWarningResponseSignature OnWarningResponse;
    FOnInspectSignature OnInspect;
    FOnInspectCompletedSignature OnInspectCompleted;

    void CreateWidgets(const TMap<EObjectType, FObjectData>& ObjectsMap);
    void ChangeCurrentWidget(EGameState GameState);
    void DeselectPlacementObject();
    void Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes);
    bool ShowWarning(const FText& WarningText);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> WelcomeWidgetClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> GameWidgetClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAnimatedWidget> InspectWidgetClasses;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EWidgetType, TObjectPtr<UDP_BaseAnimatedWidget>> Widgets;
    UPROPERTY()
    TObjectPtr<UDP_BaseAnimatedWidget> CurrentWidget;
    EWidgetType CurrentWidgetType{EWidgetType::Welcome};

    [[nodiscard]] FORCEINLINE UDP_GameWidget* GetGameWidget() const;
    FORCEINLINE void SetCurrentWidget();
    FORCEINLINE void HandleGameWidget(EGameState GameState);

    void OnObjectTypeChangedHandler(EObjectType ObjectType);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
    void OnDestroyAllHandler();
    void OnDestroySelectedHandler();
    void OnFadeoutAnimationFinishedHandler();
    void OnQuitHandler();
    void OnToggleScreenModeHandler();
    void OnShowHelpHandler();
    void OnWarningResponseHandler(bool bCondition);
    void OnInspectHandler();
    void OnInspectCompletedHandler();
};
