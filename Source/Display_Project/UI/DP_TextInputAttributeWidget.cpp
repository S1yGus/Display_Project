// Display_Project, all rights reserved.

#include "UI/DP_TextInputAttributeWidget.h"
#include "Components/EditableText.h"
#include "DP_Utils.h"

void UDP_TextInputAttributeWidget::Update(const FAttributeData& Data)
{
    Super::Update(Data);

    const auto NewText = FText::FromString(Data.Get<FString>());
    TextInput->SetText(NewText);
}

void UDP_TextInputAttributeWidget::Reset()
{
    Super::Reset();

    const auto NewText = UI::GetAdjustedDefaultText(GetType(), DefaultText);
    TextInput->SetText(NewText);
    OnTextChanged(NewText);
}

void UDP_TextInputAttributeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(TextInput);
    TextInput->OnTextChanged.AddDynamic(this, &ThisClass::OnTextChanged);
}

void UDP_TextInputAttributeWidget::OnTextChanged(const FText& Text)
{
    FAttributeData Data;
    Data.Set<FString>(Text.ToString());
    OnAttributeChanged.Broadcast(GetType(), Data);
}
