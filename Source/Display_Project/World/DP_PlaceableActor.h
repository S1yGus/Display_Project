// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_CoreEnums.h"
#include "DP_CoreAliases.h"
#include "DP_PlaceableActor.generated.h"

class UMaterialInterface;
class UDP_FXComponent;

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
    [[nodiscard]] FGuid GetObjectGuid() const { return Guid; }

    void Init(FAttributesMap&& Attributes, FGuid&& InGuid);
    void Init(const FAttributesMap& Attributes, const FGuid& InGuid);
    void UpdateAttribute(EAttributeType AttributeType, FAttributeData AttributeData);
    virtual void UpdateAttributes();

    virtual void Interact(const FTransform& InteractionTransform);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Select")
    void Select();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Select")
    void Deselect();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview")
    void UpdatePreviewMode(bool bEnabled);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview")
    void UpdatePreviewMaterial(UMaterialInterface* PreviewMaterial);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UDP_FXComponent> FXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
    FIntPoint Size{1};

    EObjectType Type{EObjectType::None};
    FAttributesMap AttributesMap;

private:
    FGuid Guid;

    void SetAttribute(EAttributeType AttributeType, FAttributeData AttributeData);
};
