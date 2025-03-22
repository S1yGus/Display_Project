// Display_Project, all rights reserved.

#include "World/DP_Display_8.h"
#include "World/DP_Segment.h"

ADP_Display_8::ADP_Display_8()
{
    // MeshComponent->Rename(TEXT("BodyMesh"));

    GlassMesh = CreateDefaultSubobject<UStaticMeshComponent>("GlassMesh");
    check(GlassMesh);
    GlassMesh->SetupAttachment(MeshComponent);

    CREATE_SEGMENT(Segment_2);
    CREATE_SEGMENT(Segment_3);
    CREATE_SEGMENT(Segment_4);
    CREATE_SEGMENT(Segment_5);
    CREATE_SEGMENT(Segment_6);
    CREATE_SEGMENT(Segment_7);
    CREATE_SEGMENT(Segment_8);
}

void ADP_Display_8::SetPreviewMode(bool bEnabled)
{
    Super::SetPreviewMode(bEnabled);

    PlaceableActor::SetMeshPreviewMode(bEnabled, GlassMesh, DefaultGlassMaterials);
}

void ADP_Display_8::UpdateMaterial(UMaterialInterface* Material)
{
    Super::UpdateMaterial(Material);

    for (int32 i = 0; i < GlassMesh->GetNumMaterials(); ++i)
    {
        GlassMesh->SetMaterial(i, Material);
    }
}

void ADP_Display_8::BeginPlay()
{
    Super::BeginPlay();

    DefaultGlassMaterials.Reserve(GlassMesh->GetNumMaterials());
    for (int32 i = 0; i < GlassMesh->GetNumMaterials(); ++i)
    {
        DefaultGlassMaterials.Add(GlassMesh->GetMaterial(i));
    }
}
