// Display_Project, all rights reserved.

#include "UI/DP_InspectWidget.h"
#include "UI/DP_ButtonWidget.h"

void UDP_InspectWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(BackButton);

    BackButton->OnClicked.AddUObject(this, &ThisClass::OnClickedBackButtonHandler);
}

void UDP_InspectWidget::OnClickedBackButtonHandler()
{
    OnInspectCompleted.Broadcast();
}
