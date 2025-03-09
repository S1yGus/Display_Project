// Display_Project, all rights reserved.

#include "DP_CoreStructures.h"

void FBytesAttribute::Sterilize(const FAttributeData& InAttributeData)
{
    FMemoryWriter Writer(Bytes);
    bool bIsString{InAttributeData.IsType<FString>()};
    Writer << bIsString;
    if (bIsString)
    {
        FString TempString = InAttributeData.Get<FString>();
        Writer << TempString;
    }
    else
    {
        FGuid TempGuid = InAttributeData.Get<FGuid>();
        Writer << TempGuid;
    }
}

void FBytesAttribute::Deserialize(FAttributeData& OutAttributeData) const
{
    FMemoryReader Reader(Bytes);
    bool bIsString;
    Reader << bIsString;
    if (bIsString)
    {
        FString TempString;
        Reader << TempString;
        OutAttributeData.Set<FString>(TempString);
    }
    else
    {
        FGuid TempGuid;
        Reader << TempGuid;
        OutAttributeData.Set<FGuid>(TempGuid);
    }
}

void FObjectSaveData::SterilizeAttributes(const FAttributesMap& InAttributes)
{
    for (const auto& [AttributeType, AttributeData] : InAttributes)
    {
        if (!Attributes.Contains(AttributeType))
        {
            Attributes.Add(AttributeType);
        }
        Attributes[AttributeType].Sterilize(AttributeData);
    }
}

void FObjectSaveData::DeserializeAttributes(FAttributesMap& OutAttributes) const
{
    for (auto& [AttributeType, AttributeBytes] : Attributes)
    {
        if (!OutAttributes.Contains(AttributeType))
        {
            OutAttributes.Add(AttributeType);
        }
        AttributeBytes.Deserialize(OutAttributes[AttributeType]);
    }
}
