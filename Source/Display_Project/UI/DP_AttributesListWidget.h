// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_AttributesListWidget.generated.h"

class UVerticalBox;
class UDP_BaseAttributeWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_AttributesListWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnAttributeChangedSignature OnAttributeChanged;

    void Init(EObjectType ObjectType, const FAttributesMap& Attributes);
    void Reset();

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> AttributesBox;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAttributeWidget> ComboBoxAttributeWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_BaseAttributeWidget> TextInputAttributeWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    EObjectType Type;

    [[nodiscard]] TObjectPtr<UDP_BaseAttributeWidget> CreateAttributeWidget(EAttributeType AttributeType, const FAttribute& Attribute);
    [[nodiscard]] TObjectPtr<UDP_BaseAttributeWidget> CreateTextInputAttributeWidget(const FText& DefaultText);
    [[nodiscard]] TObjectPtr<UDP_BaseAttributeWidget> CreateComboBoxAttributeWidget(FComboBoxDataUpdater&& UpdateFunc);

    void OnAttributeChangedHandler(EAttributeType Type, FAttributeData Data);
};
