// Display_Project, all rights reserved.

#include "World/DP_Segment.h"
#include "UI/DP_TextWidget.h"
#include "Components/WidgetComponent.h"

ADP_Segment::ADP_Segment()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("SegmentText");
    check(WidgetComponent);
    WidgetComponent->SetCastShadow(false);
    WidgetComponent->SetupAttachment(RootComponent);
}

void ADP_Segment::Set(TCHAR Character)
{
    TextWidget->SetText(FText::FromString(FString{1, &Character}));
}

void ADP_Segment::Clear()
{
    TextWidget->SetText(FText::GetEmpty());
}

void ADP_Segment::Teardown()
{
    Destroy();
}

void ADP_Segment::BeginPlay()
{
    Super::BeginPlay();

    TextWidget = Cast<UDP_TextWidget>(WidgetComponent->GetWidget());
    check(TextWidget);
}
