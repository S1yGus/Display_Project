// Display_Project, all rights reserved.

#include "Framework/DP_GameModeBase.h"
#include "Framework/DP_PlayerController.h"
#include "Framework/DP_HUD.h"
#include "Framework/DP_Player.h"
#include "UObject/ConstructorHelpers.h"
#include "Save/DP_GameSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"

ADP_GameModeBase::ADP_GameModeBase()
{
    static ConstructorHelpers::FClassFinder<ADP_PlayerController> PC(TEXT("/Game/Framework/BP_PlayerController"));
    if (PC.Class)
    {
        PlayerControllerClass = PC.Class;
    }

    static ConstructorHelpers::FClassFinder<ADP_HUD> HUD(TEXT("/Game/Framework/BP_HUD"));
    if (HUD.Class)
    {
        HUDClass = HUD.Class;
    }

    static ConstructorHelpers::FClassFinder<ADP_Player> Player(TEXT("/Game/Framework/BP_Player"));
    if (Player.Class)
    {
        DefaultPawnClass = Player.Class;
    }
}

bool ADP_GameModeBase::AddSaveRecord(FSaveRecord&& SaveRecord)
{
    if (GameSave)
    {
        GameSave->AddSaveRecord(MoveTemp(SaveRecord));
        return UGameplayStatics::SaveGameToSlot(GameSave, GameSaveSlotName, 0);
    }

    return false;
}

bool ADP_GameModeBase::DeleteSaveRecord(const FGuid& Guid)
{
    if (GameSave)
    {
        if (GameSave->DeleteSaveRecord(Guid))
        {
            return UGameplayStatics::SaveGameToSlot(GameSave, GameSaveSlotName, 0);
        }
    }

    return false;
}

TOptional<FSaveRecord> ADP_GameModeBase::GetSaveRecord(const FGuid& Guid) const
{
    if (GameSave)
    {
        return GameSave->GetSaveRecord(Guid);
    }

    return NullOpt;
}

TArray<FSaveRecordMetaData> ADP_GameModeBase::GetSaveRecordsMetaData() const
{
    if (GameSave)
    {
        return GameSave->GetSaveRecordsMetaData();
    }

    return {};
}

void ADP_GameModeBase::StartPlay()
{
    Super::StartPlay();

    CheckGameSave();
    PreloadAssets();
}

void ADP_GameModeBase::CheckGameSave()
{
    if (UGameplayStatics::DoesSaveGameExist(GameSaveSlotName, 0))
    {
        GameSave = Cast<UDP_GameSave>(UGameplayStatics::LoadGameFromSlot(GameSaveSlotName, 0));
    }
    else
    {
        GameSave = Cast<UDP_GameSave>(UGameplayStatics::CreateSaveGameObject(UDP_GameSave::StaticClass()));
    }

    check(GameSave);
}

void ADP_GameModeBase::PreloadAssets()
{
    if (!AssetsToPreload.IsEmpty())
    {
        UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetsToPreload,
                                                               FStreamableDelegate::CreateUObject(this, &ADP_GameModeBase::OnAssetsPreloadCompletedHandler));
    }
    else
    {
        OnAssetsPreloadCompletedHandler();
    }
}

void ADP_GameModeBase::OnAssetsPreloadCompletedHandler()
{
    OnAssetsPreloadCompleted.Broadcast();
}
