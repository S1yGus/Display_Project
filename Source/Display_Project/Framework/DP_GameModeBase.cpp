// Display_Project, all rights reserved.

#include "Framework/DP_GameModeBase.h"
#include "Framework/DP_PlayerController.h"
#include "Framework/DP_HUD.h"
#include "UObject/ConstructorHelpers.h"

ADP_GameModeBase::ADP_GameModeBase()
{
    static ConstructorHelpers::FClassFinder<ADP_PlayerController> PC(TEXT("/Game/Framework/BP_PlayerController"));
    if (PC.Class)
    {
        PlayerControllerClass = PC.Class;
    }

    static ConstructorHelpers::FClassFinder<ADP_HUD> HUD(TEXT("/Game/Framework/BP_HUD"));
    if (HUD.Class)
    {
        HUDClass = HUD.Class;
    }
}
