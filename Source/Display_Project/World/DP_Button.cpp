// Display_Project, all rights reserved.

#include "World/DP_Button.h"
#include "World/DP_Display_1.h"

ADP_Button::ADP_Button()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADP_Button::Interact()
{
    if (Display)
    {
        Display->RefreshText(Text);
    }
}
