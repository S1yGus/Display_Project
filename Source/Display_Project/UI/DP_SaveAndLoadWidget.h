// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_AnimatedWidgetWithWarning.h"
#include "DP_CoreDelegates.h"
#include "DP_SaveAndLoadWidget.generated.h"

class UDP_ButtonWidget;
class UDP_SaveRecordRowWidget;
class UEditableText;
class UVerticalBox;

UCLASS()
class DISPLAY_PROJECT_API UDP_SaveAndLoadWidget : public UDP_AnimatedWidgetWithWarning
{
    GENERATED_BODY()

public:
    FOnSaveSignature OnSave;
    FOnLoadSignature OnLoad;
    FOnDeleteSaveSignature OnDeleteSave;
    FOnBackSignature OnBack;

    void UpdateSaves(const TArray<FSaveRecordMetaData>& SaveRecordsMetaData);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> SaveRecordsVerticalBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UEditableText> TextInput;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> SaveButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> LoadButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> DeleteButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> BackButton;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_SaveRecordRowWidget> SaveRecordRowWidgetClasses;

    virtual void NativeOnInitialized() override;

private:
    FGuid SelectedRecordGuid;

    FORCEINLINE bool SetSelectdSaveRecord();

    UFUNCTION()
    void OnInputTextChangedHandler(const FText& Text);
    void OnClickedSaveButtonHandler();
    void OnClickedLoadButtonHandler();
    void OnClickedDeleteButtonHandler();
    void OnClickedBackButtonHandler();
    void OnSelectedSaveRecordHandler(const FGuid& Guid);
};
