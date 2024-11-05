// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreDelegates.h"
#include "DP_BaseAnimatedWidget.generated.h"

UCLASS()
class DISPLAY_PROJECT_API UDP_BaseAnimatedWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnFadeoutAnimationFinishedSignature OnFadeoutAnimationFinished;

    void ShowStartupAnimation();
    void ShowFadeoutAnimation();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> StartupAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> FadeoutAnimation;

    virtual void NativeOnInitialized() override;
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
