// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_CoreEnums.h"
#include "DP_CoreAliases.h"
#include "DP_PlaceableActor.generated.h"

class UMaterialInterface;

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_PlaceableActor : public AActor
{
    GENERATED_BODY()

public:
    ADP_PlaceableActor();

    [[nodiscard]] EObjectType GetObjectType() const { return Type; };
    [[nodiscard]] FString GetObjectName() const;
    [[nodiscard]] const FAttributesMap& GetObjectAttributes() const { return AttributesMap; };
    [[nodiscard]] FIntPoint GetObjectSize() const { return Size; }

    void Init(FAttributesMap&& Attributes);
    void Init(const FAttributesMap& Attributes);
    void UpdateAttribute(EAttributeType AttributeType, FAttributeData AttributeData);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Select")
    void Select(const FTransform& SelectionTransform);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Select")
    void Deselect();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview")
    void UpdatePreviewMode(bool bEnabled);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview")
    void UpdatePreviewMaterial(UMaterialInterface* PreviewMaterial);

protected:
    EObjectType Type{EObjectType::None};
    FAttributesMap AttributesMap;

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
    FIntPoint Size{1};

    virtual void UpdateAttributes();

private:
    void SetAttribute(EAttributeType AttributeType, FAttributeData AttributeData);
};
