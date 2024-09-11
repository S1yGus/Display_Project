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
class ADP_GameModeBase;

UCLASS()
class DISPLAY_PROJECT_API UDP_PlacementWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UHorizontalBox> ButtonsBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> AttributesSwitcher;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> FreeAllButton;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_ObjectButtonWidget> ObjectButtonWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UDP_AttributesListWidget> AttributesListWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    FORCEINLINE [[nodiscard]] ADP_GameModeBase* GetGameMode() const;

    void CreateWidgetsForObjects();
    FORCEINLINE UDP_ObjectButtonWidget* CreateButtonWidget(EObjectType ObjectType, const FText& ObjectName, int32 WidgetID);
    FORCEINLINE UDP_AttributesListWidget* CreateAttributesListWidget(EObjectType ObjectType, const FAttributesMap& Attributes);
    FORCEINLINE void ResetCurrentObjectAttributes();

    UFUNCTION()
    void OnClickedFreeButton();
    void OnClickedObjectButton(EObjectType ObjectType, int32 WidgetID);
    void OnObjectTypeChanged(EObjectType ObjectType);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
};
