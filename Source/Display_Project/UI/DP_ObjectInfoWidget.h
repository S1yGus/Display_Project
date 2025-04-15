// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_ObjectInfoWidget.generated.h"

class UDP_ObjectHeaderWidget;
class UDP_AttributesListWidget;
class UTexture2D;

UCLASS()
class DISPLAY_PROJECT_API UDP_ObjectInfoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnAttributeChangedSignature OnAttributeChanged;

    void Init(EObjectType ObjectType, UTexture2D* Thumbnail, const TSet<EAttributeType>& Attributes);
    void UpdateAttributes(const TAttributesMap& Attributes);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ObjectHeaderWidget> ObjectHeader;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_AttributesListWidget> AttributesList;

    virtual void NativeOnInitialized() override;

private:
    void OnAttributeChangedHandler(EAttributeType AttributeType, TAttributeData AttributeData);
};
