// Display_Project, all rights reserved.

#include "UI/DP_WarningWidget.h"
#include "UI/DP_ButtonWidget.h"
#include "Components/TextBlock.h"

void UDP_WarningWidget::SetWarning(const FText& InWarningText)
{
    WarningText->SetText(InWarningText);
}

void UDP_WarningWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(WarningText);
    check(AcceptButton);
    check(DenyButton);

    AcceptButton->OnClicked.AddUObject(this, &ThisClass::OnClickedAcceptButtonHandler);
    DenyButton->OnClicked.AddUObject(this, &ThisClass::OnClickedDenyButtonHandler);
}

void UDP_WarningWidget::OnClickedAcceptButtonHandler()
{
    OnWarningResponse.Broadcast(true);
}

void UDP_WarningWidget::OnClickedDenyButtonHandler()
{
    OnWarningResponse.Broadcast(false);
}
