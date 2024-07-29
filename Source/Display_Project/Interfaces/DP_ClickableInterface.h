// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DP_ClickableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDP_ClickableInterface : public UInterface
{
    GENERATED_BODY()
};

class IDP_ClickableInterface
{
    GENERATED_BODY()

public:
    virtual void Interact() = 0;
};
