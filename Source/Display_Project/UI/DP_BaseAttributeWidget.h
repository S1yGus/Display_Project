// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_BaseAttributeWidget.generated.h"

class UTextBlock;

UCLASS(Abstract)
class DISPLAY_PROJECT_API UDP_BaseAttributeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnAttributeChangedSignature OnAttributeChanged;

    void Init(EAttributeType AttributeType, const FAttribute& Attribute);

    virtual void Reset();

protected:
    EAttributeType Type;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> AttributeName;

    virtual void NativeOnInitialized() override;
};
