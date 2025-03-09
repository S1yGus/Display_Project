// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DP_CoreStructures.h"
#include "DP_GameSave.generated.h"

UCLASS()
class DISPLAY_PROJECT_API UDP_GameSave : public USaveGame
{
    GENERATED_BODY()

public:
    void AddSaveRecord(FSaveRecord&& SaveRecord);
    bool DeleteSaveRecord(const FGuid& Guid);
    [[nodiscard]] TOptional<FSaveRecord> GetSaveRecord(const FGuid& Guid) const;
    [[nodiscard]] TArray<FSaveRecordMetaData> GetSaveRecordsMetaData() const;

private:
    UPROPERTY()
    TArray<FSaveRecord> SaveRecords;

    FORCEINLINE [[nodiscard]] const FSaveRecord* FindSaveRecordByGuid(const FGuid& Guid) const;
};
