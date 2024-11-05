// Display_Project, all rights reserved.

#include "World/DP_GridController.h"
#include "World/DP_Grid.h"
#include "World/DP_Node.h"
#include "World/DP_PlaceableActor.h"
#include "World/DP_TextShifter.h"
#include "Framework/DP_PlayerController.h"
#include "Framework/DP_HUD.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridController, All, All)

static constexpr double GirdHeight{150.0};

void ADP_GridController::BeginPlay()
{
    Super::BeginPlay();

#if WITH_EDITOR
    const UEnum* ObjectEnum = StaticEnum<EObjectType>();
    for (int32 i = 0; i < ObjectEnum->NumEnums() - 1; ++i)
    {
        auto CurrentType = static_cast<EObjectType>(i);
        if (CurrentType != EObjectType::None)
        {
            check(ObjectsMap.Contains(CurrentType));
            check(ObjectsMap[CurrentType].Class);
        }
    }
#endif    // WITH_EDITOR

    SpawnGrid();

    if (auto* PC = GetPlayerController())
    {
        PC->OnUpdatePreviewLocation.AddUObject(this, &ThisClass::OnUpdatePreviewLocationHandler);
        PC->OnObjectSpawn.AddUObject(this, &ThisClass::OnSpawnCurrentObjectHandler);
        PC->OnObjectSelected.AddUObject(this, &ThisClass::OnSelectHandler);
        PC->OnWelcomeScreenCompleted.AddUObject(this, &ThisClass::OnSwitchToGameHandler);
        PC->UpdatePlayerLocation(WelcomePoint->GetActorLocation());
    }

    if (auto* HUD = GetHUD())
    {
        HUD->OnObjectTypeChanged.AddUObject(this, &ThisClass::SetCurrentObjectType);
        HUD->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
        HUD->OnDestroySelected.AddUObject(this, &ThisClass::OnDestroySelectedHandler);
        HUD->OnDestroyAll.AddUObject(this, &ThisClass::OnDestroyAllHandler);
        HUD->OnQuit.AddUObject(this, &ThisClass::OnQuitHandler);
        HUD->OnToggleScreenMode.AddUObject(this, &ThisClass::OnToggleScreenModeHandler);
        HUD->OnShowHelp.AddUObject(this, &ThisClass::OnShowHelpHandler);
        HUD->OnWarningResponse.AddUObject(this, &ThisClass::OnWarningResponseHandler);
        HUD->CreateWidgets(ObjectsMap);
    }

    FTimerHandle WelcomeTimerHandle;
    GetWorldTimerManager().SetTimer(
        WelcomeTimerHandle,
        [this]()
        {
            SetGameState_Internal(EGameState::Welcome);
        },
        WelcomeDelay, false);
}

ADP_PlayerController* ADP_GridController::GetPlayerController() const
{
    return GetWorld() ? GetWorld()->GetFirstPlayerController<ADP_PlayerController>() : nullptr;
}

ADP_HUD* ADP_GridController::GetHUD() const
{
    return GetPlayerController() ? GetPlayerController()->GetHUD<ADP_HUD>() : nullptr;
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
        else
        {
            DeferredAction();
            DeferredAction = nullptr;
        }
    }
}

void ADP_GridController::SpawnGrid()
{
    Grid = GetWorld()->SpawnActor<ADP_Grid>(GridClass, FVector{0.0, 0.0, GirdHeight}, FRotator::ZeroRotator);
    check(Grid);
    Grid->OnObjectSpawned.AddUObject(this, &ThisClass::OnObjectSpawnedHandler);
}

void ADP_GridController::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    SetGameState_Internal(NewGameState);
}

void ADP_GridController::SetGameState_Internal(EGameState NewGameState)
{
    PrevGameState = CurrentGameState;
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
    SetGameState(PrevGameState);
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

void ADP_GridController::OnObjectSpawnedHandler()
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

void ADP_GridController::OnSelectHandler(AActor* SelectedActor, const FTransform& SelectionTransform)
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
        SelectedObject->Select(SelectionTransform);

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
    Grid->Free(SelectedObject);
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
    // TODO
}

void ADP_GridController::OnShowHelpHandler()
{
    UpdatePlayerLocation(WelcomePoint->GetActorLocation());
    SetGameState(EGameState::Welcome);
}

void ADP_GridController::OnWarningResponseHandler(bool bCondition)
{
    SetGameState(PrevGameState);

    if (DeferredAction && bCondition)
    {
        DeferredAction();
        DeferredAction = nullptr;
    }
}
