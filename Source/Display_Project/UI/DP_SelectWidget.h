// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_SelectWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UDP_ObjectInfoWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_SelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnAttributeChangedSignature OnAttributeChanged;
    FOnDestroySelectedSignature OnDestroySelected;

    void CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap);
    void Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> AttributesSwitcher;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> InspectButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> DestroyButton;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_ObjectInfoWidget> ObjectInfoWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    TMap<EObjectType, int32> TypeIDMap;

    UFUNCTION()
    void OnClickedInspectButtonHandler();
    UFUNCTION()
    void OnClickedDestroyButtonHandler();
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
};
