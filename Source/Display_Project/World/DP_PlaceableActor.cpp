// Display_Project, all rights reserved.

#include "World/DP_PlaceableActor.h"
#include "Components/DP_FXComponent.h"
#include "DP_CoreStructures.h"

void PlaceableActor::SetMeshPreviewMode(bool bEnabled, UStaticMeshComponent* Mesh, const TArray<UMaterialInterface*>& DefaultMeshMaterials)
{
    if (!bEnabled)
    {
        for (int32 i = 0; i < Mesh->GetNumMaterials() && i < DefaultMeshMaterials.Num(); ++i)
        {
            Mesh->SetMaterial(i, DefaultMeshMaterials[i]);
        }
    }

    Mesh->SetRenderCustomDepth(bEnabled);
    Mesh->SetCastShadow(!bEnabled);
}

ADP_PlaceableActor::ADP_PlaceableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    check(MeshComponent);
    MeshComponent->SetupAttachment(RootComponent);

    FXComponent = CreateDefaultSubobject<UDP_FXComponent>("FXComponent");
    check(FXComponent);
}

FString ADP_PlaceableActor::GetObjectName() const
{
    if (AttributesMap.Contains(EAttributeType::ObjectName))
    {
        return AttributesMap[EAttributeType::ObjectName].Get<FString>();
    }
    return GetName();
}

void ADP_PlaceableActor::Init(FAttributesMap&& Attributes, FGuid&& InGuid)
{
    AttributesMap = MoveTemp(Attributes);
    Guid = MoveTemp(InGuid);
    UpdateAttributes();
}

void ADP_PlaceableActor::Init(const FAttributesMap& Attributes, const FGuid& InGuid)
{
    AttributesMap = Attributes;
    Guid = InGuid;
    UpdateAttributes();
}

void ADP_PlaceableActor::UpdateAttribute(EAttributeType AttributeType, FAttributeData AttributeData)
{
    SetAttribute(AttributeType, AttributeData);
    UpdateAttributes();
}

void ADP_PlaceableActor::UpdateAttributes()
{
}

void ADP_PlaceableActor::SetPreviewMode(bool bEnabled)
{
    PlaceableActor::SetMeshPreviewMode(bEnabled, MeshComponent, DefaultMeshMaterials);
}

void ADP_PlaceableActor::UpdateMaterial(UMaterialInterface* Material)
{
    for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
    {
        MeshComponent->SetMaterial(i, Material);
    }
}

void ADP_PlaceableActor::Interact(const FTransform& InteractionTransform)
{
    FXComponent->MakeInteractFX(InteractionTransform);
}

void ADP_PlaceableActor::SetSelected(bool bSelected)
{
    MeshComponent->SetOverlayMaterial(bSelected ? OverlayMaterial : nullptr);
}

void ADP_PlaceableActor::BeginPlay()
{
    Super::BeginPlay();

    DefaultMeshMaterials.Reserve(MeshComponent->GetNumMaterials());
    for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
    {
        DefaultMeshMaterials.Add(MeshComponent->GetMaterial(i));
    }
}

void ADP_PlaceableActor::SetAttribute(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (!AttributesMap.Contains(AttributeType))
    {
        AttributesMap.Add(AttributeType);
    }
    AttributesMap[AttributeType] = AttributeData;
}
