// Display_Project, all rights reserved.

#include "Framework/DP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/DP_InteractiveInterface.h"
#include "Framework/DP_GameModeBase.h"

ADP_PlayerController::ADP_PlayerController()
{
    bShowMouseCursor = true;
    bEnableMouseOverEvents = true;
}

void ADP_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));

    if (auto* GameMode = GetGameMode())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void ADP_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (const auto* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();    //
            InputSystem && InputMapping)
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }
    }

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        Input->BindAction(ClickAction, ETriggerEvent::Started, this, &ThisClass::OnClick);
        Input->BindAction(SelectAction, ETriggerEvent::Started, this, &ThisClass::OnSelect);
    }
}

ADP_GameModeBase* ADP_PlayerController::GetGameMode()
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ADP_GameModeBase>() : nullptr;
}

void ADP_PlayerController::ObjectPlacementClick()
{
    if (auto* GameMode = GetGameMode())
    {
        GameMode->SpawnCurrentObject();
    }
}

void ADP_PlayerController::InteractClick()
{
    if (FHitResult HitResult; GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery3, false, HitResult))    // ECC_GameTraceChannel1 "Clickable"
    {
        if (auto* ClickableActor = Cast<IDP_InteractiveInterface>(HitResult.GetActor()))
        {
            ClickableActor->Interact(FTransform{HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint});
        }
    }
}

void ADP_PlayerController::OnClick()
{
    switch (CurrentGameState)
    {
        case EGameState::ObjectPlacement:
            ObjectPlacementClick();
            break;
        default:
            InteractClick();
            break;
    }
}

void ADP_PlayerController::OnSelect()
{
    // TODO
}

void ADP_PlayerController::OnGameStateChanged(EGameState NewGameState)
{
    CurrentGameState = NewGameState;
}
