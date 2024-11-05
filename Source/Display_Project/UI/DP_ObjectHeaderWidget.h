// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreEnums.h"
#include "DP_ObjectHeaderWidget.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;

UCLASS()
class DISPLAY_PROJECT_API UDP_ObjectHeaderWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(EObjectType Type, UTexture2D* InThumbnail);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ObjectType;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UImage> Thumbnail;

    virtual void NativeOnInitialized() override;
};
