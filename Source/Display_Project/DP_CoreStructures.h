// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"
#include "World/DP_PlaceableActor.h"
#include "DP_CoreStructures.generated.h"

class UTexture2D;

USTRUCT()
struct FObjectData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TObjectPtr<UTexture2D> Thumbnail;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADP_PlaceableActor> Class;

    UPROPERTY(EditAnywhere)
    TSet<EAttributeType> Attributes;
};

USTRUCT()
struct FVideoQualityOptionsData
{
    GENERATED_USTRUCT_BODY()

    TArray<FString> VideoQualityOptions;
    FString CurrentVideoQualityOption;
};
