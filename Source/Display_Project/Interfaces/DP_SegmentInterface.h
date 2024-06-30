// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DP_SegmentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDP_SegmentInterface : public UInterface
{
    GENERATED_BODY()
};

class IDP_SegmentInterface
{
    GENERATED_BODY()

public:
    virtual void Set(TCHAR Character) = 0;
    virtual void Clear() = 0;
};
