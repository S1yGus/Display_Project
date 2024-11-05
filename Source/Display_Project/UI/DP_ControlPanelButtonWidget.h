// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreDelegates.h"
#include "DP_ControlPanelButtonWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_ControlPanelButtonWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedSignature OnClicked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FText Text{FText::GetEmpty()};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FLinearColor ButtonTextColor{1.0f, 1.0f, 1.0f, 1.0f};

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> Button;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ButtonText;

    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;

private:
    UFUNCTION()
    void OnClickedHandler();
};
