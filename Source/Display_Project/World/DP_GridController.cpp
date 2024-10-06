// Display_Project, all rights reserved.

#include "World/DP_GridController.h"
#include "World/DP_Grid.h"
#include "World/DP_Node.h"
#include "World/DP_PlaceableActor.h"
#include "Framework/DP_PlayerController.h"
#include "Framework/DP_HUD.h"

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

    if (auto* PC = GetWorld()->GetFirstPlayerController<ADP_PlayerController>())
    {
        PC->OnUpdatePreviewLocation.AddUObject(this, &ThisClass::OnUpdatePreviewLocationHandler);
        PC->OnRequestObjectSpawn.AddUObject(this, &ThisClass::OnSpawnCurrentObjectHandler);
        PC->OnObjectSelected.AddUObject(this, &ThisClass::OnSelectHandler);

        if (auto* HUD = PC->GetHUD<ADP_HUD>())
        {
            HUD->OnObjectTypeChanged.AddUObject(this, &ThisClass::SetCurrentObjectType);
            HUD->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
            HUD->OnDestroySelected.AddUObject(this, &ThisClass::OnDestroySelectedHandler);
            HUD->OnDestroyAll.AddUObject(this, &ThisClass::OnDestroyAllHandler);
            HUD->CreateWidgets(ObjectsMap);
        }
    }

    SetCurrentObjectType_Internal(EObjectType::None);
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
    CurrentGameState = NewGameState;

    if (auto* PC = GetWorld()->GetFirstPlayerController<ADP_PlayerController>())
    {
        PC->UpdateGameState(CurrentGameState);

        if (auto* HUD = PC->GetHUD<ADP_HUD>())
        {
            HUD->ChangeCurrentWidget(CurrentGameState);
        }
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
        if (auto* PC = GetWorld()->GetFirstPlayerController<ADP_PlayerController>())
        {
            if (auto* HUD = PC->GetHUD<ADP_HUD>())
            {
                HUD->HideWidgetAttributes();
                Grid->UpdateCurrentObjectClass(nullptr);
                SetGameState(EGameState::Interact);
            }
        }
    }
    else if (ObjectsMap.Contains(CurrentObjectType))
    {
        Grid->UpdateCurrentObjectClass(ObjectsMap[CurrentObjectType].Class);
        SetGameState(EGameState::Placement);
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
        if (auto* PC = GetWorld()->GetFirstPlayerController<ADP_PlayerController>())
        {
            if (auto* HUD = PC->GetHUD<ADP_HUD>())
            {
                HUD->Select(SelectedObject->GetObjectType(), SelectedObject->GetObjectName(), SelectedObject->GetObjectAttributes());
            }
        }
        SetGameState(EGameState::Select);
    }
    else
    {
        if (SelectedObject)
        {
            SelectedObject->Deselect();
        }

        SetGameState(EGameState::Interact);
    }
}

void ADP_GridController::OnDestroySelectedHandler()
{
    Grid->Free(SelectedObject);
    SetGameState(EGameState::Interact);
}

void ADP_GridController::OnDestroyAllHandler()
{
    Grid->FreeAll();
}
