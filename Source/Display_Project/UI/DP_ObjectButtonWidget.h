// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_ObjectButtonWidget.generated.h"

class UDP_ButtonWidget;
class UTexture2D;
class UImage;

UCLASS()
class DISPLAY_PROJECT_API UDP_ObjectButtonWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedObjectButtonSignature OnClickedObjectButton;

    void Init(EObjectType ObjectType, UTexture2D* Thumbnail);
    void UpdateSelection(EObjectType ObjectType);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UImage> FrameImage;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> Button;

    virtual void NativeOnInitialized() override;

private:
    EObjectType Type;

    void OnClickedHandler();
};
