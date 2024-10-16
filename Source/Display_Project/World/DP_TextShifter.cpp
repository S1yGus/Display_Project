// Display_Project, all rights reserved.

#include "World/DP_TextShifter.h"
#include "Components/TextRenderComponent.h"

ADP_TextShifter::ADP_TextShifter()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    TextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
    check(TextRender);
    TextRender->SetupAttachment(GetRootComponent());
}

void ADP_TextShifter::InitShift()
{
    FTimerHandle ShiftTimerHandle;
    GetWorldTimerManager().SetTimer(ShiftTimerHandle, this, &ThisClass::OnShiftHandler, ShiftDelay, false);
}

void ADP_TextShifter::OnShiftHandler()
{
    TextRender->SetText(ShiftText);
}
