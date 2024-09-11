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
    void SetUpdateFunc(FComboBoxDataUpdater&& UpdateFunc) { Update = MoveTemp(UpdateFunc); };

    virtual void Reset() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UComboBoxString> ComboBox;

    virtual void NativeOnInitialized() override;

private:
    FComboBoxDataUpdater Update;
    TArray<FAttributeData> AttributeData;

    UFUNCTION()
    void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
