// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DP_CoreEnums.h"
#include "DP_CoreAliases.h"
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
struct FBytesAttribute
{
    GENERATED_USTRUCT_BODY()

    void Sterilize(const TAttributeData& InAttributeData);
    void Deserialize(TAttributeData& OutAttributeData) const;

private:
    UPROPERTY()
    TArray<uint8> Bytes;
};

USTRUCT()
struct FObjectSaveData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    EObjectType Type;

    UPROPERTY()
    FGuid Guid;

    UPROPERTY()
    FTransform Transform;

    void SterilizeAttributes(const TAttributesMap& InAttributes);
    void DeserializeAttributes(TAttributesMap& OutAttributes) const;

private:
    UPROPERTY()
    TMap<EAttributeType, FBytesAttribute> Attributes;
};

USTRUCT()
struct FVideoQualityOptionsData
{
    GENERATED_USTRUCT_BODY()

    TArray<FString> VideoQualityOptions;

    FString CurrentVideoQualityOption;
};

USTRUCT()
struct FSaveRecordMetaData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FText Name;

    UPROPERTY()
    FGuid Guid;
};

USTRUCT()
struct FSaveRecordData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FObjectSaveData> ObjectData;

    UPROPERTY()
    TArray<FGuid> NodesState;
};

USTRUCT()
struct FSaveRecord
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FSaveRecordMetaData MetaData;

    UPROPERTY()
    FSaveRecordData Data;
};
