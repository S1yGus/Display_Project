// Display_Project, all rights reserved.

#include "World/DP_Button.h"
#include "World/DP_Display_1.h"

ADP_Button::ADP_Button()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    check(RootComponent);
}

void ADP_Button::Interact(const FTransform& InteractionTransform)
{
    if (!bIsInteracted)
    {
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
