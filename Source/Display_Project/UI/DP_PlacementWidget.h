// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_PlacementWidget.generated.h"

class UButton;
class UHorizontalBox;
class UWidgetSwitcher;
class UDP_ObjectButtonWidget;
class UDP_AttributesListWidget;

UCLASS()
class DISPLAY_PROJECT_API UDP_PlacementWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnObjectTypeChangedSignature OnObjectTypeChanged;
    FOnAttributeChangedSignature OnAttributeChanged;
    FOnDestroyAllSignature OnDestroyAll;

    void CreateWidgetsForObjects(const TMap<EObjectType, FObjectData>& ObjectsMap);
    void HideAttributes();

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UHorizontalBox> ButtonsBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> AttributesSwitcher;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> DestroyAllButton;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_ObjectButtonWidget> ObjectButtonWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_AttributesListWidget> AttributesListWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    FORCEINLINE UDP_ObjectButtonWidget* CreateButtonWidget(EObjectType ObjectType, int32 WidgetID);
    FORCEINLINE UDP_AttributesListWidget* CreateAttributesListWidget(EObjectType ObjectType, const TArray<EAttributeType>& Attributes);
    FORCEINLINE void ResetCurrentAttributesList();

    UFUNCTION()
    void OnClickedDestroyAllButtonHandler();
    void OnClickedObjectButtonHandler(EObjectType ObjectType, int32 WidgetID);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
};
