// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_SelectWidget.generated.h"

class UWidgetSwitcher;
class UDP_ButtonWidget;
class UDP_ObjectInfoWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_SelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnAttributeChangedSignature OnAttributeChanged;
    FOnDestroySelectedSignature OnDestroySelected;
    FOnCopySignature OnCopy;
    FOnMoveSignature OnMove;
    FOnInspectSignature OnInspect;

    void CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap);
    void Select(EObjectType ObjectType, const TAttributesMap& Attributes);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> AttributesSwitcher;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> CopyButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> MoveButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> InspectButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> DestroyButton;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_ObjectInfoWidget> ObjectInfoWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    TMap<EObjectType, int32> TypeIDMap;

    void OnClickedCopyButtonHandler();
    void OnClickedMoveButtonHandler();
    void OnClickedInspectButtonHandler();
    void OnClickedDestroyButtonHandler();
    void OnAttributeChangedHandler(EAttributeType AttributeType, TAttributeData AttributeData);
};
