// Display_Project, all rights reserved.

#include "Framework/DP_GameModeBase.h"
#include "World/DP_PlaceableActor.h"
#include "GameFramework/PlayerController.h"

static const float SpawnTimerRate{0.016f};

void ADP_GameModeBase::SpawnCurrentObject()
{
    if (CurrentGameState != EGameState::ObjectPlacement)
        return;

    if (CanSpawn())
    {
        TargetScale = MaxPreviewScale;
        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawning, SpawnTimerRate, true);
    }
}

void ADP_GameModeBase::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::ObjectPlacement);
}

void ADP_GameModeBase::Tick(float DeltaSeconds)
{
    if (CurrentGameState == EGameState::ObjectPlacement)
    {
        if (auto* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
        {
            if (FHitResult HitResult; PreviewObject && PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery4, false, HitResult))
            {
                PreviewObject->SetActorLocation(HitResult.ImpactPoint);
            }
        }
    }
}

void ADP_GameModeBase::UpdatePreview()
{
    PreviewObject->UpdatePreview(CanSpawn() ? ValidPreviewMaterial : InvalidPreviewMaterial);
}

void ADP_GameModeBase::OnPreviewBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor->IsInA(ADP_PlaceableActor::StaticClass()))
    {
        ++OverlapCounter;
        UpdatePreview();
    }
}

void ADP_GameModeBase::OnPreviewEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor->IsInA(ADP_PlaceableActor::StaticClass()))
    {
        --OverlapCounter;
        UpdatePreview();
    }
}

void ADP_GameModeBase::OnSpawning()
{
    PreviewObject->SetActorScale3D(FMath::VInterpTo(PreviewObject->GetActorScale3D(), TargetScale, SpawnTimerRate, ScaleInterpSpeed));
    if (PreviewObject->GetActorScale3D().Equals(MaxPreviewScale))
    {
        TargetScale = MinPreviewScale;
    }
    else if (PreviewObject->GetActorScale3D().Equals(MinPreviewScale))
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        GetWorld()->SpawnActor<ADP_PlaceableActor>(ObjectClasses[CurrentObjectType], FTransform{PreviewObject->GetActorRotation(), PreviewObject->GetActorLocation()});
        PreviewObject->Destroy();
        // SetGameState(EGameState::WaitingToStart);
    }
}

void ADP_GameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    OnGameStateChanged.Broadcast(NewGameState);

    switch (NewGameState)
    {
        case EGameState::ObjectPlacement:
            if (GetWorld() && ObjectClasses.Contains(CurrentObjectType))
            {
                const FVector PreviewScale{1.01};
                const FTransform SpawnTransform{FRotator::ZeroRotator, FVector::ZeroVector, PreviewScale};
                PreviewObject = GetWorld()->SpawnActor<ADP_PlaceableActor>(ObjectClasses[CurrentObjectType], SpawnTransform);
                check(PreviewObject);
                PreviewObject->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnPreviewBeginOverlap);
                PreviewObject->OnActorEndOverlap.AddDynamic(this, &ThisClass::OnPreviewEndOverlap);
                UpdatePreview();
            }
            break;
        default:
            break;
    }
}
