// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_ButtonWidget.h"
#include "DP_TextButtonWidget.generated.h"

class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_TextButtonWidget : public UDP_ButtonWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FText Text{FText::GetEmpty()};

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ButtonText;

    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;
};
