// Display_Project, all rights reserved.

#include "UI/DP_BaseAnimatedWidget.h"

void UDP_BaseAnimatedWidget::ShowStartupAnimation()
{
    PlayAnimation(StartupAnimation);
}

void UDP_BaseAnimatedWidget::ShowFadeoutAnimation()
{
    PlayAnimation(FadeoutAnimation);
}

void UDP_BaseAnimatedWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(StartupAnimation);
    check(FadeoutAnimation);
}

void UDP_BaseAnimatedWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        OnFadeoutAnimationFinished.Broadcast();
    }
}
