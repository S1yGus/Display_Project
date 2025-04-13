// Display_Project, all rights reserved.

#include "UI/DP_ComboBoxAttributeWidget.h"
#include "Components/ComboBoxString.h"

void UDP_ComboBoxAttributeWidget::Update(const FAttributeData& Data)
{
    Super::Update(Data);

    Reset();

    if (ComboBoxSelectedOption)
    {
        ComboBox->SetSelectedOption(ComboBoxSelectedOption(Data));
    }
}

void UDP_ComboBoxAttributeWidget::Reset()
{
    Super::Reset();

    if (UpdateComboBox)
    {
        UpdateComboBox(ComboBox, AttributeData);
    }
}

void UDP_ComboBoxAttributeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ComboBox);
    ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChangedHandler);
}

void UDP_ComboBoxAttributeWidget::OnSelectionChangedHandler(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (const auto Index = ComboBox->FindOptionIndex(SelectedItem); Index != INDEX_NONE)
    {
        OnAttributeChanged.Broadcast(GetType(), AttributeData[ComboBox->FindOptionIndex(SelectedItem)]);
    }
}
