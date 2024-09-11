// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"
#include "DP_CoreAliases.h"
#include "World/DP_PlaceableActor.h"
#include "DP_CoreStructures.generated.h"

USTRUCT()
struct FAttribute
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly)
    FText Name;

    FAttributeData Data;
};

USTRUCT()
struct FObjectData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ADP_PlaceableActor> Class;

    UPROPERTY(EditDefaultsOnly)
    FText Name;

    UPROPERTY(EditDefaultsOnly)
    TMap<EAttributeType, FAttribute> AttributesMap;
};
