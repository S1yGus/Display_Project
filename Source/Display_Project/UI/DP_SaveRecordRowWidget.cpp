// Display_Project, all rights reserved.

#include "UI/DP_SaveRecordRowWidget.h"
#include "Components/Image.h"

void UDP_SaveRecordRowWidget::Init(const FText& SaveName, const FGuid& SaveGuid)
{
    Text = SaveName;
    Guid = SaveGuid;
}

bool UDP_SaveRecordRowWidget::SetSelectd(const FGuid& SelectdGuid)
{
    const bool bSelected = Guid == SelectdGuid;
    Frame->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    return bSelected;
}

void UDP_SaveRecordRowWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    OnClicked.AddUObject(this, &ThisClass::OnClickedHandler);
}

void UDP_SaveRecordRowWidget::OnClickedHandler()
{
    OnSelectedSaveRecord.Broadcast(Guid);
}
