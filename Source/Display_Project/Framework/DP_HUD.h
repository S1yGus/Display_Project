// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DP_CoreTypes.h"
#include "DP_HUD.generated.h"

UCLASS()
class DISPLAY_PROJECT_API ADP_HUD : public AHUD
{
    GENERATED_BODY()

public:
    FOnObjectTypeChangedSignature OnObjectTypeChanged;
    FOnAttributeChangedSignature OnAttributeChanged;
    FOnDestroySelectedSignature OnDestroySelected;
    FOnDestroyAllSignature OnDestroyAll;

    void CreateWidgets(const TMap<EObjectType, FObjectData>& ObjectsMap);
    void ChangeCurrentWidget(EGameState GameState);
    void HideWidgetAttributes();
    void Select(EObjectType ObjectType, const FString& ObjectName, const FAttributesMap& Attributes);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UUserWidget> PlacementWidgetClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<UUserWidget> SelectWidgetClasses;

private:
    UPROPERTY()
    TMap<EGameState, TObjectPtr<UUserWidget>> GameWidgets;
    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

    void OnObjectTypeChangedHandler(EObjectType ObjectType);
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
    void OnDestroyAllHandler();
    void OnDestroySelectedHandler();
};
