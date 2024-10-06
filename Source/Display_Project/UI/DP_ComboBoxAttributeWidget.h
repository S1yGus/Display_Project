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
    void SetUpdateFunc(FComboBoxDataUpdater&& UpdateFunc) { UpdateComboBox = MoveTemp(UpdateFunc); };
    void SetSelectedOptionGetter(FComboBoxSelectedOptionGetter&& SelectedOptionGetter) { ComboBoxSelectedOption = MoveTemp(SelectedOptionGetter); }

    virtual void Update(const FAttributeData& Data) override;
    virtual void Reset() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UComboBoxString> ComboBox;

    virtual void NativeOnInitialized() override;

private:
    FComboBoxDataUpdater UpdateComboBox;
    FComboBoxSelectedOptionGetter ComboBoxSelectedOption;
    TArray<FAttributeData> AttributeData;

    UFUNCTION()
    void OnSelectionChangedHandler(FString SelectedItem, ESelectInfo::Type SelectionType);
};
