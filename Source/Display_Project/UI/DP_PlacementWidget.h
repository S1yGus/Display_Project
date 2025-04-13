// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreTypes.h"
#include "DP_PlacementWidget.generated.h"

class UHorizontalBox;
class UWidgetSwitcher;
class UDP_ButtonWidget;
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
    void HideAttributesList();
    void Copy(EObjectType ObjectType, const FAttributesMap& Attributes);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UHorizontalBox> ButtonsBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> AttributesSwitcher;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> DestroyAllButton;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_ObjectButtonWidget> ObjectButtonWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_AttributesListWidget> AttributesListWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    TMap<EObjectType, int32> TypeIDMap;

    FORCEINLINE UDP_ObjectButtonWidget* CreateButtonWidget(EObjectType ObjectType, UTexture2D* Thumbnail);
    FORCEINLINE UDP_AttributesListWidget* CreateAttributesListWidget(EObjectType ObjectType, const TSet<EAttributeType>& Attributes);
    FORCEINLINE void ResetCurrentAttributesList();
    FORCEINLINE void UpdateCurrentAttributesList(const FAttributesMap& Attributes);
    FORCEINLINE void UpdateSelection(EObjectType ObjectType);
    FORCEINLINE void UpdateButtonsSelection(EObjectType ObjectType);

    void OnClickedDestroyAllButtonHandler();
    void OnClickedObjectButtonHandler(EObjectType ObjectType);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
};
