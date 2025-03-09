// Display_Project, all rights reserved.

#include "World/DP_Panel.h"

ADP_Panel::ADP_Panel()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    PanelMesh = CreateDefaultSubobject<UStaticMeshComponent>("PanelMesh");
    check(PanelMesh);
    PanelMesh->SetupAttachment(GetRootComponent());
}

void ADP_Panel::SetLabel(const FText& NewLabel)
{
    Label = NewLabel;
    UpdatePanelLabel(Label);
}

void ADP_Panel::BeginPlay()
{
    Super::BeginPlay();

    SpawnScrews();
    UpdatePanelLabel(Label);
}

void ADP_Panel::SpawnScrews()
{
    check(ScrewClass);

    for (int32 i = 0; i < ScrewNum; ++i)
    {
        auto* Screw = GetWorld()->SpawnActor<AActor>(ScrewClass, FTransform::Identity);
        check(Screw);
        const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
        Screw->AttachToComponent(PanelMesh, AttachmentRule, FName{ScrewSocketNamePattern + FString::FromInt(i)});
    }
}
