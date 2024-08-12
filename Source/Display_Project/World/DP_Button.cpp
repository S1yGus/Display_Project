// Display_Project, all rights reserved.

#include "World/DP_Button.h"
#include "World/DP_Display_1.h"

ADP_Button::ADP_Button()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADP_Button::Interact(const FTransform& InteractionTransform)
{
    if (!bIsInteracted)
    {
        Super::Interact(InteractionTransform);

        if (Display)
        {
            Display->RefreshText(DisplayText);
        }

        OnInteract();
    }
}

void ADP_Button::BeginPlay()
{
    Super::BeginPlay();

    SetButtonText(ButtonText);
}
