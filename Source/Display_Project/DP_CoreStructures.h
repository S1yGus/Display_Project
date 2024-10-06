// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"
#include "World/DP_PlaceableActor.h"
#include "DP_CoreStructures.generated.h"

USTRUCT()
struct FObjectData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADP_PlaceableActor> Class;

    UPROPERTY(EditAnywhere)
    TArray<EAttributeType> Attributes;
};
