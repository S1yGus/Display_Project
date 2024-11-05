// Display_Project, all rights reserved.

#include "UI/DP_AnimatedWidgetWithWarning.h"
#include "UI/DP_WarningWidget.h"

void UDP_AnimatedWidgetWithWarning::ShowWarning(const FText& WarningText)
{
    WarningWidget->SetWarning(WarningText);
    WarningWidget->SetVisibility(ESlateVisibility::Visible);
    WarningWidget->ShowStartupAnimation();
}

void UDP_AnimatedWidgetWithWarning::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(WarningWidget);

    WarningWidget->OnWarningResponse.AddUObject(this, &ThisClass::OnWarningResponseHandler);
    WarningWidget->OnFadeoutAnimationFinished.AddUObject(this, &ThisClass::OnWarningFadeoutAnimationFinishedHandler);
}

void UDP_AnimatedWidgetWithWarning::OnWarningResponseHandler(bool bCondition)
{
    WarningWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
    WarningWidget->ShowFadeoutAnimation();
    OnWarningResponse.Broadcast(bCondition);
}

void UDP_AnimatedWidgetWithWarning::OnWarningFadeoutAnimationFinishedHandler()
{
    WarningWidget->SetVisibility(ESlateVisibility::Hidden);
}
