// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_BaseAttributeWidget.h"
#include "DP_CoreTypes.h"
#include "DP_ComboBoxAttributeWidget.generated.h"

class UComboBoxString;

UCLASS()
class DISPLAY_PROJECT_API UDP_ComboBoxAttributeWidget : public UDP_BaseAttributeWidget
{
    GENERATED_BODY()

public:
    void SetUpdateFunc(TComboBoxDataUpdater&& UpdateFunc) { UpdateComboBox = MoveTemp(UpdateFunc); };
    void SetSelectedOptionGetter(TComboBoxSelectedOptionGetter&& SelectedOptionGetter) { ComboBoxSelectedOption = MoveTemp(SelectedOptionGetter); }

    virtual void Update(const TAttributeData& Data) override;
    virtual void Reset() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UComboBoxString> ComboBox;

    virtual void NativeOnInitialized() override;

private:
    TComboBoxDataUpdater UpdateComboBox;
    TComboBoxSelectedOptionGetter ComboBoxSelectedOption;
    TArray<TAttributeData> AttributeData;

    UFUNCTION()
    void OnSelectionChangedHandler(FString SelectedItem, ESelectInfo::Type SelectionType);
};
