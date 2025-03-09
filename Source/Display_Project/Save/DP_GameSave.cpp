// Display_Project, all rights reserved.

#include "Save/DP_GameSave.h"

void UDP_GameSave::AddSaveRecord(FSaveRecord&& SaveRecord)
{
    SaveRecords.Emplace(MoveTemp(SaveRecord));
}

bool UDP_GameSave::DeleteSaveRecord(const FGuid& Guid)
{
    return static_cast<bool>(SaveRecords.RemoveAll(
        [&Guid](const auto& SaveRecord)
        {
            return SaveRecord.MetaData.Guid == Guid;
        }));
}

TOptional<FSaveRecord> UDP_GameSave::GetSaveRecord(const FGuid& Guid) const
{
    if (const auto* Found = FindSaveRecordByGuid(Guid))
    {
        return *Found;
    }

    return NullOpt;
}

TArray<FSaveRecordMetaData> UDP_GameSave::GetSaveRecordsMetaData() const
{
    return Algo::Accumulate(SaveRecords, TArray<FSaveRecordMetaData>(),
                            [](auto&& Array, const auto& SaveRecord)
                            {
                                Array.Add(SaveRecord.MetaData);
                                return MoveTemp(Array);
                            });
}

const FSaveRecord* UDP_GameSave::FindSaveRecordByGuid(const FGuid& Guid) const
{
    return SaveRecords.FindByPredicate(
        [&Guid](const auto& SaveRecord)
        {
            return SaveRecord.MetaData.Guid == Guid;
        });
}
