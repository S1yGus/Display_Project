// Display_Project, all rights reserved.

#include "Framework/DP_GameModeBase.h"
#include "World/DP_Grid.h"
#include "World/DP_Node.h"

static constexpr double GirdHeight{150.0};

void ADP_GameModeBase::StartPlay()
{
    Super::StartPlay();

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

    SetCurrentObjectType_Internal(EObjectType::None);
    SetGameState_Internal(EGameState::Placement);
}

void ADP_GameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    SetGameState_Internal(NewGameState);
}

void ADP_GameModeBase::SetCurrentObjectType(EObjectType NewObjectType)
{
    if (CurrentObjectType == NewObjectType)
        return;

    SetCurrentObjectType_Internal(NewObjectType);
}

void ADP_GameModeBase::AddCurrentObjectAttribute(EAttributeType AttributeType, FAttributeData AttributeData)
{
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}

void ADP_GameModeBase::SpawnCurrentObject()
{
    Grid->SpawnCurrentObject();
}

void ADP_GameModeBase::UpdatePreviewLocation(AActor* ReferenceActor)
{
    Grid->UpdatePreviewLocation(Cast<ADP_Node>(ReferenceActor));
}

void ADP_GameModeBase::SpawnGrid()
{
    Grid = GetWorld()->SpawnActor<ADP_Grid>(GridClass, FVector{0.0, 0.0, GirdHeight}, FRotator::ZeroRotator);
    check(Grid);
}

void ADP_GameModeBase::SetGameState_Internal(EGameState NewGameState)
{
    CurrentGameState = NewGameState;
    OnGameStateChanged.Broadcast(NewGameState);
}

void ADP_GameModeBase::SetCurrentObjectType_Internal(EObjectType NewObjectType)
{
    CurrentObjectType = NewObjectType;
    OnObjectTypeChanged.Broadcast(NewObjectType);
}