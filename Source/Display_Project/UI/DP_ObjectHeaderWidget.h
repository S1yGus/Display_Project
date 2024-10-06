// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreEnums.h"
#include "DP_ObjectHeaderWidget.generated.h"

class UTextBlock;

UCLASS()
class DISPLAY_PROJECT_API UDP_ObjectHeaderWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(EObjectType Type);
    void UpdateObjectName(const FString& Name);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ObjectType;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ObjectName;

    virtual void NativeOnInitialized() override;
};
