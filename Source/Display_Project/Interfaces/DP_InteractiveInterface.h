// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DP_InteractiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDP_InteractiveInterface : public UInterface
{
    GENERATED_BODY()
};

class IDP_InteractiveInterface
{
    GENERATED_BODY()

public:
    virtual void Interact(const FTransform& InteractionTransform) = 0;
};
