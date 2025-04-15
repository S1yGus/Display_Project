// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_BaseAttributeWidget.h"
#include "DP_TextInputAttributeWidget.generated.h"

class UEditableText;

UCLASS()
class DISPLAY_PROJECT_API UDP_TextInputAttributeWidget : public UDP_BaseAttributeWidget
{
    GENERATED_BODY()

public:
    void SetDefaultText(const FText& Text) { DefaultText = Text; };

    virtual void Update(const TAttributeData& Data) override;
    virtual void Reset() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UEditableText> TextInput;

    virtual void NativeOnInitialized() override;

private:
    FText DefaultText{FText::GetEmpty()};

    UFUNCTION()
    void OnTextChanged(const FText& Text);
};
