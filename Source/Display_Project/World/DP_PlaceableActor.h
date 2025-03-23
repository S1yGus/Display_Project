// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_CoreEnums.h"
#include "DP_CoreAliases.h"
#include "DP_PlaceableActor.generated.h"

class UMaterialInterface;
class UDP_FXComponent;

namespace PlaceableActor
{

void SetMeshPreviewMode(bool bEnabled, UStaticMeshComponent* Mesh, const TArray<UMaterialInterface*>& DefaultMeshMaterials);

}    // namespace PlaceableActor

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
    virtual void SetPreviewMode(bool bEnabled);
    virtual void UpdateMaterial(UMaterialInterface* Material);
    virtual void Interact(const FTransform& InteractionTransform);
    virtual void SetSelected(bool bSelected);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_FXComponent> FXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
    FIntPoint Size{1};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    TObjectPtr<UMaterialInterface> OverlayMaterial;

    EObjectType Type{EObjectType::None};
    FAttributesMap AttributesMap;

    virtual void BeginPlay() override;

private:
    FGuid Guid;
    UPROPERTY()
    TArray<UMaterialInterface*> DefaultMeshMaterials;

    FORCEINLINE void CollectDefaultMeshMaterials();
    void SetAttribute(EAttributeType AttributeType, FAttributeData AttributeData);
};
