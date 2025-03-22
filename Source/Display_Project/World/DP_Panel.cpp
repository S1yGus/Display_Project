// Display_Project, all rights reserved.

#include "World/DP_Panel.h"
#include "UI/DP_TextWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetStringLibrary.h"

ADP_Panel::ADP_Panel()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    PanelMesh = CreateDefaultSubobject<UStaticMeshComponent>("PanelMesh");
    check(PanelMesh);
    PanelMesh->SetupAttachment(RootComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ButtonText");
    check(WidgetComponent);
    WidgetComponent->SetCastShadow(false);
    WidgetComponent->SetupAttachment(PanelMesh);
}

void ADP_Panel::SetLabel(const FString& NewLabel)
{
    Label = NewLabel;
    UpdateLabel(Label);
}

void ADP_Panel::BeginPlay()
{
    Super::BeginPlay();

    SpawnScrews();
    UpdateLabel(Label);
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

void ADP_Panel::UpdateLabel(const FString& NewLabel)
{
    if (auto* TextWidget = Cast<UDP_TextWidget>(WidgetComponent->GetWidget()))
    {
        const auto NewPanelLabel = UKismetStringLibrary::GetSubstring(NewLabel, 0, MaxLabelLength);
        TextWidget->SetText(FText::FromString(NewPanelLabel));
    }
}
