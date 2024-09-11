// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_ObjectButtonWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_ObjectButtonWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedObjectButtonSignature OnClickedObjectButton;

    void Init(EObjectType ObjectType, const FText& ObjectName, int32 WidgetID);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> Button;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ButtonText;

    virtual void NativeOnInitialized() override;

private:
    EObjectType Type;
    int32 ID;

    UFUNCTION()
    void OnClicked();
};
