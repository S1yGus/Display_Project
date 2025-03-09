// Display_Project, all rights reserved.

#include "UI/DP_SaveAndLoadWidget.h"
#include "UI/DP_ButtonWidget.h"
#include "UI/DP_SaveRecordRowWidget.h"
#include "Components/EditableText.h"
#include "Components/VerticalBox.h"

void UDP_SaveAndLoadWidget::UpdateSaves(const TArray<FSaveRecordMetaData>& SaveRecordsMetaData)
{
    SaveRecordsVerticalBox->ClearChildren();

    for (const auto& [Name, Guid] : SaveRecordsMetaData)
    {
        auto* SaveRecordRow = CreateWidget<UDP_SaveRecordRowWidget>(GetWorld(), SaveRecordRowWidgetClasses);
        check(SaveRecordRow);
        SaveRecordRow->Init(Name, Guid);
        SaveRecordRow->OnSelectedSaveRecord.AddUObject(this, &ThisClass::OnSelectedSaveRecordHandler);
        SaveRecordsVerticalBox->AddChild(SaveRecordRow);
    }

    const bool bSelected = SetSelectdSaveRecord();
    LoadButton->SetIsEnabled(bSelected);
    DeleteButton->SetIsEnabled(bSelected);
}

void UDP_SaveAndLoadWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SaveRecordsVerticalBox);
    check(TextInput);
    check(SaveButton);
    check(LoadButton);
    check(DeleteButton);
    check(BackButton);

    TextInput->OnTextChanged.AddDynamic(this, &ThisClass::OnInputTextChangedHandler);
    SaveButton->SetIsEnabled(false);
    SaveButton->OnClicked.AddUObject(this, &ThisClass::OnClickedSaveButtonHandler);
    LoadButton->SetIsEnabled(false);
    LoadButton->OnClicked.AddUObject(this, &ThisClass::OnClickedLoadButtonHandler);
    DeleteButton->SetIsEnabled(false);
    DeleteButton->OnClicked.AddUObject(this, &ThisClass::OnClickedDeleteButtonHandler);
    BackButton->OnClicked.AddUObject(this, &ThisClass::OnClickedBackButtonHandler);
}

bool UDP_SaveAndLoadWidget::SetSelectdSaveRecord()
{
    bool bSelected{false};
    for (auto* Widget : SaveRecordsVerticalBox->GetAllChildren())
    {
        if (auto* Record = Cast<UDP_SaveRecordRowWidget>(Widget))
        {
            if (Record->SetSelectd(SelectedRecordGuid))
            {
                bSelected = true;
            }
        }
    }

    return bSelected;
}

void UDP_SaveAndLoadWidget::OnInputTextChangedHandler(const FText& Text)
{
    SaveButton->SetIsEnabled(!Text.IsEmpty());
}

void UDP_SaveAndLoadWidget::OnClickedSaveButtonHandler()
{
    OnSave.Broadcast(TextInput->GetText());

    TextInput->SetText(FText::GetEmpty());
    SaveButton->SetIsEnabled(false);
}

void UDP_SaveAndLoadWidget::OnClickedLoadButtonHandler()
{
    OnLoad.Broadcast(SelectedRecordGuid);
}

void UDP_SaveAndLoadWidget::OnClickedDeleteButtonHandler()
{
    OnDeleteSave.Broadcast(SelectedRecordGuid);
}

void UDP_SaveAndLoadWidget::OnClickedBackButtonHandler()
{
    OnBack.Broadcast();
}

void UDP_SaveAndLoadWidget::OnSelectedSaveRecordHandler(const FGuid& Guid)
{
    SelectedRecordGuid = Guid;
    const bool bSelected = SetSelectdSaveRecord();
    LoadButton->SetIsEnabled(bSelected);
    DeleteButton->SetIsEnabled(bSelected);
}
