// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DP_CoreTypes.h"
#include "DP_GameModeBase.generated.h"

class UDP_GameSave;

UCLASS()
class DISPLAY_PROJECT_API ADP_GameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    FOnAssetsPreloadCompletedSignature OnAssetsPreloadCompleted;

    ADP_GameModeBase();

    bool AddSaveRecord(FSaveRecord&& SaveRecord);
    bool DeleteSaveRecord(const FGuid& Guid);
    [[nodiscard]] TOptional<FSaveRecord> GetSaveRecord(const FGuid& Guid) const;
    [[nodiscard]] TArray<FSaveRecordMetaData> GetSaveRecordsMetaData() const;

    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    FString GameSaveSlotName{"GameSave"};

    UPROPERTY(EditAnywhere, Category = "Preload")
    TArray<FSoftObjectPath> AssetsToPreload;

private:
    UPROPERTY()
    TObjectPtr<UDP_GameSave> GameSave;

    void CheckGameSave();
    void PreloadAssets();
    void OnAssetsPreloadCompletedHandler();
};
