// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_TextWidget.generated.h"

class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_TextWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetText(const FText& Text);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> WidgetText;

    virtual void NativeOnInitialized() override;
};
