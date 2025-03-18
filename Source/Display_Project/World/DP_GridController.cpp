// Display_Project, all rights reserved.

#include "World/DP_GridController.h"
#include "World/DP_Grid.h"
#include "World/DP_Node.h"
#include "World/DP_PlaceableActor.h"
#include "World/DP_TextShifter.h"
#include "Framework/DP_PlayerController.h"
#include "Framework/DP_Player.h"
#include "Framework/DP_HUD.h"
#include "Framework/DP_GameUserSettings.h"
#include "Framework/DP_GameModeBase.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridController, All, All)

static constexpr double GirdHeight{150.0};

static TArray<FObjectSaveData> GetAllObjectsSaveData(const UObject* WorldContextObject)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ADP_PlaceableActor::StaticClass(), FoundActors);

    TArray<FObjectSaveData> SaveObjectData;
    SaveObjectData.Reserve(FoundActors.Num());
    for (const auto* Actor : FoundActors)
    {
        if (const auto* PlaceableActor = Cast<ADP_PlaceableActor>(Actor))
        {
            FObjectSaveData ObjectData;
            ObjectData.Type = PlaceableActor->GetObjectType();
            ObjectData.SterilizeAttributes(PlaceableActor->GetObjectAttributes());
            ObjectData.Guid = PlaceableActor->GetObjectGuid();
            ObjectData.Transform = PlaceableActor->GetActorTransform();
            SaveObjectData.Add(ObjectData);
        }
    }

    return SaveObjectData;
}

void ADP_GridController::BeginPlay()
{
    Super::BeginPlay();

#if WITH_EDITOR
    const UEnum* ObjectEnum = StaticEnum<EObjectType>();
    for (int32 i = 0; i < ObjectEnum->NumEnums() - 1; ++i)
    {
        const auto CurrentType = static_cast<EObjectType>(i);
        if (CurrentType != EObjectType::None)
        {
            check(ObjectsMap.Contains(CurrentType));
            check(ObjectsMap[CurrentType].Class);
        }
    }
#endif    // WITH_EDITOR

    SetGameState_Internal(EGameState::Preload);

    SetupGameMode();
    SetupPlayerController();
    SetupHUD();
    SpawnGrid();
}

ADP_PlayerController* ADP_GridController::GetPlayerController() const
{
    return GetWorld() ? GetWorld()->GetFirstPlayerController<ADP_PlayerController>() : nullptr;
}

ADP_Player* ADP_GridController::GetPlayer() const
{
    return GetPlayerController() ? GetPlayerController()->GetPawn<ADP_Player>() : nullptr;
}

ADP_HUD* ADP_GridController::GetHUD() const
{
    return GetPlayerController() ? GetPlayerController()->GetHUD<ADP_HUD>() : nullptr;
}

ADP_GameModeBase* ADP_GridController::GetGameMode() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ADP_GameModeBase>() : nullptr;
}

void ADP_GridController::UpdatePlayerLocation(const FVector& Location)
{
    if (auto* PC = GetPlayerController())
    {
        PC->UpdatePlayerLocation(Location);
    }
}

void ADP_GridController::ShowWarning(const FText& WarningText, FDeferredAction&& Action)
{
    if (auto* HUD = GetHUD())
    {
        DeferredAction = MoveTemp(Action);

        if (HUD->ShowWarning(WarningText))
        {
            SetGameState(EGameState::Warning);
        }
        else if (DeferredAction)
        {
            DeferredAction();
            DeferredAction = nullptr;
        }
    }
}

void ADP_GridController::UpdateUISaveRecords()
{
    if (const auto* GameMode = GetGameMode())
    {
        if (auto* HUD = GetHUD())
        {
            HUD->UpdateSaves(GameMode->GetSaveRecordsMetaData());
        }
    }
}

void ADP_GridController::SetupGameMode()
{
    if (auto* GameMode = GetGameMode())
    {
        GameMode->OnAssetsPreloadCompleted.AddUObject(this, &ThisClass::OnAssetsPreloadCompletedHandler);
    }
}

void ADP_GridController::SetupPlayerController()
{
    if (auto* PC = GetPlayerController())
    {
        PC->OnUpdatePreviewLocation.AddUObject(this, &ThisClass::OnUpdatePreviewLocationHandler);
        PC->OnObjectSpawn.AddUObject(this, &ThisClass::OnSpawnCurrentObjectHandler);
        PC->OnObjectSelected.AddUObject(this, &ThisClass::OnSelectHandler);
        PC->OnWelcomeScreenCompleted.AddUObject(this, &ThisClass::OnSwitchToGameHandler);
        PC->UpdatePlayerLocation(WelcomePoint->GetActorLocation());
    }
}

void ADP_GridController::SetupHUD()
{
    if (auto* HUD = GetHUD())
    {
        HUD->OnObjectTypeChanged.AddUObject(this, &ThisClass::SetCurrentObjectType);
        HUD->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
        HUD->OnDestroySelected.AddUObject(this, &ThisClass::OnDestroySelectedHandler);
        HUD->OnDestroyAll.AddUObject(this, &ThisClass::OnDestroyAllHandler);
        HUD->OnQuit.AddUObject(this, &ThisClass::OnQuitHandler);
        HUD->OnToggleScreenMode.AddUObject(this, &ThisClass::OnToggleScreenModeHandler);
        HUD->OnShowOptions.AddUObject(this, &ThisClass::OnShowOptionsHandler);
        HUD->OnBack.AddUObject(this, &ThisClass::OnBackHandler);
        HUD->OnShowSaveAndLoad.AddUObject(this, &ThisClass::OnShowSaveAndLoadHandler);
        HUD->OnSave.AddUObject(this, &ThisClass::OnSaveHandler);
        HUD->OnLoad.AddUObject(this, &ThisClass::OnLoadHandler);
        HUD->OnDeleteSave.AddUObject(this, &ThisClass::OnDeleteSaveHandler);
        HUD->OnShowHelp.AddUObject(this, &ThisClass::OnShowHelpHandler);
        HUD->OnWarningResponse.AddUObject(this, &ThisClass::OnWarningResponseHandler);
        HUD->OnInspect.AddUObject(this, &ThisClass::OnInspectHandler);
        HUD->OnInspectCompleted.AddUObject(this, &ThisClass::OnInspectCompletedHandler);
        HUD->OnVideoQualityChanged.AddUObject(this, &ThisClass::OnVideoQualityChangedHandler);
        HUD->OnRotationSpeedChanged.AddUObject(this, &ThisClass::OnRotationSpeedChangedHandler);
        HUD->OnSoundVolumeChanged.AddUObject(this, &ThisClass::OnSoundVolumeChangedHandler);

        if (const auto* GameUserSettings = UDP_GameUserSettings::Get())
        {
            HUD->CreateWidgets(ObjectsMap,                                        //
                               GameUserSettings->GetVideoQualityOptionsData(),    //
                               GameUserSettings->GetRotationSpeedNormalized(),    //
                               GameUserSettings->GetSoundVolume());
        }
    }
}

void ADP_GridController::SpawnGrid()
{
    Grid = GetWorld()->SpawnActor<ADP_Grid>(GridClass, FVector{0.0, 0.0, GirdHeight}, FRotator::ZeroRotator);
    check(Grid);
    Grid->OnObjectSpawnCompleted.AddUObject(this, &ThisClass::OnObjectSpawnCompletedHandler);
}

void ADP_GridController::InitWelcomeState()
{
    FTimerHandle WelcomeTimerHandle;
    GetWorldTimerManager().SetTimer(
        WelcomeTimerHandle,
        [this]()
        {
            SetGameState_Internal(EGameState::Welcome);
        },
        WelcomeDelay, false);
}

void ADP_GridController::SpawnObjectsFromSave(const TArray<FObjectSaveData>& SaveObjectData)
{
    if (!GetWorld())
        return;

    TArray<ADP_PlaceableActor*> SpawnedPlaceableActors;
    SpawnedPlaceableActors.Reserve(SaveObjectData.Num());

    for (const auto& ObjectData : SaveObjectData)
    {
        if (ObjectsMap.Contains(ObjectData.Type))
        {
            auto* PlaceableActor = GetWorld()->SpawnActor<ADP_PlaceableActor>(ObjectsMap[ObjectData.Type].Class, ObjectData.Transform);
            check(PlaceableActor);
            FAttributesMap Attributes;
            ObjectData.DeserializeAttributes(Attributes);
            PlaceableActor->Init(Attributes, ObjectData.Guid);
            SpawnedPlaceableActors.Add(PlaceableActor);
        }
    }

    for (auto* PlaceableActor : SpawnedPlaceableActors)
    {
        PlaceableActor->UpdateAttributes();
    }
}

void ADP_GridController::SetGameState(EGameState NewGameState)
{
    if (NewGameState == CurrentGameState)
        return;

    if (NewGameState == EGameState::Interact)
    {
        PrevGameStates.Empty();
    }
    else
    {
        PrevGameStates.Add(CurrentGameState);
    }

    SetGameState_Internal(NewGameState);
}

void ADP_GridController::SetPrevGameState()
{
    if (!PrevGameStates.IsEmpty())
    {
        SetGameState_Internal(PrevGameStates.Pop());
    }
}

void ADP_GridController::SetGameState_Internal(EGameState NewGameState)
{
    CurrentGameState = NewGameState;

    if (auto* PC = GetPlayerController())
    {
        PC->UpdateGameState(CurrentGameState);
    }

    if (auto* HUD = GetHUD())
    {
        HUD->ChangeCurrentWidget(CurrentGameState);
    }
}

void ADP_GridController::SetCurrentObjectType(EObjectType NewObjectType)
{
    if (CurrentObjectType == NewObjectType)
        return;

    SetCurrentObjectType_Internal(NewObjectType);
}

void ADP_GridController::SetCurrentObjectType_Internal(EObjectType NewObjectType)
{
    CurrentObjectType = NewObjectType;

    if (CurrentObjectType == EObjectType::None)
    {
        if (auto* HUD = GetHUD())
        {
            HUD->DeselectPlacementObject();
            Grid->UpdateCurrentObjectClass(nullptr);
            SetGameState(EGameState::Interact);
        }
    }
    else if (ObjectsMap.Contains(CurrentObjectType))
    {
        Grid->UpdateCurrentObjectClass(ObjectsMap[CurrentObjectType].Class);
        SetGameState(EGameState::Placement);
    }
}

void ADP_GridController::OnSwitchToGameHandler()
{
    SetGameState(EGameState::Interact);
    UpdatePlayerLocation(SelectedObject ? SelectedObject->GetActorLocation() : Grid->GetActorLocation());

    for (auto Text : WelcomeText)
    {
        Text->InitShift();
    }
}

void ADP_GridController::OnUpdatePreviewLocationHandler(AActor* ReferenceActor)
{
    Grid->UpdatePreviewLocation(Cast<ADP_Node>(ReferenceActor));
}

void ADP_GridController::OnSpawnCurrentObjectHandler()
{
    Grid->SpawnCurrentObject();
}

void ADP_GridController::OnObjectSpawnCompletedHandler()
{
    SetCurrentObjectType(EObjectType::None);
}

void ADP_GridController::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (CurrentGameState == EGameState::Placement)
    {
        Grid->AddCurrentObjectAttribute(AttributeType, AttributeData);
    }
    else if (CurrentGameState == EGameState::Select)
    {
        SelectedObject->UpdateAttribute(AttributeType, AttributeData);
    }
}

void ADP_GridController::OnSelectHandler(AActor* SelectedActor)
{
    if (CurrentGameState == EGameState::Placement)
    {
        SetCurrentObjectType(EObjectType::None);
    }
    else if (auto* PlaceableActor = Cast<ADP_PlaceableActor>(SelectedActor))
    {
        if (SelectedObject)
        {
            SelectedObject->Deselect();
        }
        SelectedObject = PlaceableActor;
        SelectedObject->Select();

        if (auto* HUD = GetHUD())
        {
            HUD->Select(SelectedObject->GetObjectType(), SelectedObject->GetObjectName(), SelectedObject->GetObjectAttributes());
        }

        UpdatePlayerLocation(SelectedObject->GetActorLocation());
        SetGameState(EGameState::Select);
    }
    else
    {
        if (SelectedObject)
        {
            SelectedObject->Deselect();
            SelectedObject = nullptr;
        }

        UpdatePlayerLocation(Grid->GetActorLocation());
        SetGameState(EGameState::Interact);
    }
}

void ADP_GridController::OnDestroySelectedHandler()
{
    Grid->Free(SelectedObject->GetObjectGuid());
    SelectedObject = nullptr;

    UpdatePlayerLocation(Grid->GetActorLocation());
    SetGameState(EGameState::Interact);
}

void ADP_GridController::OnDestroyAllHandler()
{
    ShowWarning(DestroyAllWarning,
                [this]()
                {
                    Grid->FreeAll();
                });
}

void ADP_GridController::OnQuitHandler()
{
    ShowWarning(QuitWarning,
                [this]()
                {
                    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Type::Quit, false);
                });
}

void ADP_GridController::OnToggleScreenModeHandler()
{
    if (auto* GameUserSettings = UDP_GameUserSettings::Get())
    {
        GameUserSettings->ToggleScreenMode();
    }
}

void ADP_GridController::OnShowOptionsHandler()
{
    SetGameState(EGameState::Options);
}

void ADP_GridController::OnBackHandler()
{
    SetPrevGameState();
}

void ADP_GridController::OnVideoQualityChangedHandler(EVideoQuality VideoQuality)
{
    if (auto* GameUserSettings = UDP_GameUserSettings::Get())
    {
        GameUserSettings->SetVideoQuality(VideoQuality);
    }
}

void ADP_GridController::OnRotationSpeedChangedHandler(float RotationSpeedNormalized)
{
    if (auto* GameUserSettings = UDP_GameUserSettings::Get())
    {
        GameUserSettings->SetRotationSpeed(RotationSpeedNormalized);
    }
}

void ADP_GridController::OnSoundVolumeChangedHandler(float SoundVolume)
{
    if (auto* GameUserSettings = UDP_GameUserSettings::Get())
    {
        GameUserSettings->SetSoundVolume(SoundVolume);
    }
}

void ADP_GridController::OnShowSaveAndLoadHandler()
{
    UpdateUISaveRecords();
    SetGameState(EGameState::SaveAndLoad);
}

void ADP_GridController::OnSaveHandler(const FText& SaveName)
{
    if (auto* GameMode = GetGameMode())
    {
        FSaveRecord SaveRecord;
        SaveRecord.MetaData.Name = SaveName;
        SaveRecord.MetaData.Guid = FGuid::NewGuid();
        SaveRecord.Data.ObjectData = GetAllObjectsSaveData(GetWorld());
        SaveRecord.Data.NodesState = Grid->GetNodesState();
        if (GameMode->AddSaveRecord(MoveTemp(SaveRecord)))
        {
            UpdateUISaveRecords();
            Grid->SetPanelLabel(SaveName);
        }
    }
}

void ADP_GridController::OnLoadHandler(const FGuid& Guid)
{
    ShowWarning(LoadWarning,
                [this, &Guid]()
                {
                    if (const auto* GameMode = GetGameMode())
                    {
                        if (const auto SaveRecord = GameMode->GetSaveRecord(Guid); SaveRecord.IsSet())
                        {
                            const auto& Data = SaveRecord->Data;
                            const auto& MetaData = SaveRecord->MetaData;
                            Grid->FreeAll();
                            Grid->UpdateNodesState(Data.NodesState);
                            Grid->SetPanelLabel(MetaData.Name);
                            SpawnObjectsFromSave(Data.ObjectData);
                            SetGameState(EGameState::Interact);
                        }
                    }
                });
}

void ADP_GridController::OnDeleteSaveHandler(const FGuid& Guid)
{
    ShowWarning(DeleteSaveWarning,
                [this, &Guid]()
                {
                    if (auto* GameMode = GetGameMode())
                    {
                        if (GameMode->DeleteSaveRecord(Guid))
                        {
                            UpdateUISaveRecords();
                        }
                    }
                });
}

void ADP_GridController::OnShowHelpHandler()
{
    UpdatePlayerLocation(WelcomePoint->GetActorLocation());
    SetGameState(EGameState::Welcome);
}

void ADP_GridController::OnWarningResponseHandler(bool bCondition)
{
    SetPrevGameState();

    if (DeferredAction && bCondition)
    {
        DeferredAction();
    }
    DeferredAction = nullptr;
}

void ADP_GridController::OnInspectHandler()
{
    SetGameState(EGameState::Inspect);

    if (auto* Player = GetPlayer())
    {
        Player->StartInspect(SelectedObject->GetClass(), SelectedObject->GetObjectAttributes());
    }
}

void ADP_GridController::OnInspectCompletedHandler()
{
    SetPrevGameState();

    if (auto* Player = GetPlayer())
    {
        Player->StopInspect();
    }
}

void ADP_GridController::OnAssetsPreloadCompletedHandler()
{
    InitWelcomeState();
}
