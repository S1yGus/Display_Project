// Display_Project, all rights reserved.

#include "Framework/DP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/DP_InteractiveInterface.h"
#include "Kismet/GameplayStatics.h"

#define ECC_Node ETraceTypeQuery::TraceTypeQuery4
#define ECC_Clickable ETraceTypeQuery::TraceTypeQuery3

ADP_PlayerController::ADP_PlayerController()
{
    bShowMouseCursor = true;
    bEnableMouseOverEvents = true;
}

void ADP_PlayerController::UpdateGameState(EGameState NewGameState)
{
    CurrentGameState = NewGameState;
}

void ADP_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (CurrentGameState == EGameState::Placement)
    {
        if (FHitResult HitResult; GetHitResultUnderCursorByChannel(ECC_Node, false, HitResult))
        {
            OnUpdatePreviewLocation.Broadcast(HitResult.GetActor());
        }
    }
}

void ADP_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
}

void ADP_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (const auto* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); InputSystem && InputMapping)
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }
    }

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        Input->BindAction(ClickAction, ETriggerEvent::Started, this, &ThisClass::OnClickHandler);
        Input->BindAction(SelectAction, ETriggerEvent::Started, this, &ThisClass::OnSelectHandler);
    }
}

void ADP_PlayerController::ObjectPlacementClick()
{
    OnRequestObjectSpawn.Broadcast();
}

void ADP_PlayerController::InteractClick()
{
    if (FHitResult HitResult; GetHitResultUnderCursorByChannel(ECC_Clickable, false, HitResult))
    {
        if (auto* ClickableActor = Cast<IDP_InteractiveInterface>(HitResult.GetActor()))
        {
            ClickableActor->Interact(FTransform{HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint});
        }
    }
}

void ADP_PlayerController::OnClickHandler()
{
    switch (CurrentGameState)
    {
        case EGameState::Placement:
            ObjectPlacementClick();
            break;
        case EGameState::Interact:
            [[fallthrough]];
        case EGameState::Select:
            InteractClick();
            break;
        default:
            break;
    }
}

void ADP_PlayerController::OnSelectHandler()
{
    if (FHitResult HitResult; GetHitResultUnderCursorByChannel(ECC_Clickable, false, HitResult))
    {
        OnObjectSelected.Broadcast(HitResult.GetActor(), FTransform{HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint});
    }
    else
    {
        OnObjectSelected.Broadcast(nullptr, {});
    }
}
