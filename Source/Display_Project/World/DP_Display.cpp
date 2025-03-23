// Display_Project, all rights reserved.

#include "World/DP_Display.h"
#include "World/DP_Segment.h"
#include "Algorithms/DP_BaseScrollingAlgorithm.h"

DEFINE_LOG_CATEGORY_STATIC(LogDisplay, All, All)

ADP_Display::ADP_Display()
{
    PrimaryActorTick.bCanEverTick = false;
    Type = EObjectType::Display;

    // MeshComponent->Rename(TEXT("BodyMesh"));

    GlassMesh = CreateDefaultSubobject<UStaticMeshComponent>("GlassMesh");
    check(GlassMesh);
    GlassMesh->SetupAttachment(MeshComponent);
}

void ADP_Display::SetPreviewMode(bool bEnabled)
{
    Super::SetPreviewMode(bEnabled);

    PlaceableActor::SetMeshPreviewMode(bEnabled, GlassMesh, DefaultGlassMaterials);
}

void ADP_Display::UpdateMaterial(UMaterialInterface* Material)
{
    Super::UpdateMaterial(Material);

    for (int32 i = 0; i < GlassMesh->GetNumMaterials(); ++i)
    {
        GlassMesh->SetMaterial(i, Material);
    }
}

void ADP_Display::BeginPlay()
{
    Super::BeginPlay();

    SpawnSegments();
    CollectDefaultGlassMaterials();
    CreateScrollingAlgorithm();
}

void ADP_Display::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    for (auto& Segment : Segments)
    {
        Segment->Teardown();
    }
}

void ADP_Display::UpdateAttributes()
{
    Super::UpdateAttributes();

    if (AttributesMap.Contains(EAttributeType::DisplayText))
    {
        RefreshText(AttributesMap[EAttributeType::DisplayText].Get<FString>());
    }
}

void ADP_Display::SpawnSegments()
{
    if (!GetWorld())
        return;

    check(SegmentClass);
    for (int32 i = 0; i < SegmentsNum; ++i)
    {
        auto* Segment = GetWorld()->SpawnActor<AActor>(SegmentClass, FTransform::Identity);
        check(Segment);
        const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
        Segment->AttachToComponent(MeshComponent, AttachmentRule, FName{SegmentSocketNamePattern + FString::FromInt(i)});
        Segments.Add(Segment);
    }
}

void ADP_Display::CollectDefaultGlassMaterials()
{
    DefaultGlassMaterials.Reserve(GlassMesh->GetNumMaterials());
    for (int32 i = 0; i < GlassMesh->GetNumMaterials(); ++i)
    {
        DefaultGlassMaterials.Add(GlassMesh->GetMaterial(i));
    }
}

void ADP_Display::CreateScrollingAlgorithm()
{
    check(ScrollingAlgorithmClass);
    ScrollingAlgorithm = NewObject<UDP_BaseScrollingAlgorithm>(this, ScrollingAlgorithmClass);
    check(ScrollingAlgorithm);
    ScrollingAlgorithm->StartScrolling(Segments, DisplayText.ToUpper());
}

void ADP_Display::RefreshText(const FString& Text)
{
    if (IsValid(ScrollingAlgorithm))
    {
        DisplayText = Text.ToUpper();
        ScrollingAlgorithm->RefreshText(DisplayText);
    }
}
