// Display_Project, all rights reserved.

#include "UI/DP_ComboBoxAttributeWidget.h"
#include "Components/ComboBoxString.h"

void UDP_ComboBoxAttributeWidget::Reset()
{
    ComboBox->ClearOptions();
    AttributeData.Empty();
    if (Update)
    {
        Update(ComboBox, AttributeData);
    }
}

void UDP_ComboBoxAttributeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ComboBox);
    ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChanged);
}

void UDP_ComboBoxAttributeWidget::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    OnAttributeChanged.Broadcast(Type, AttributeData[ComboBox->FindOptionIndex(SelectedItem)]);
}
