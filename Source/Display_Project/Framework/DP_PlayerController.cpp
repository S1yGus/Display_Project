// Display_Project, all rights reserved.

#include "Framework/DP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#define TraceTypeQueryNode ETraceTypeQuery::TraceTypeQuery4
#define TraceTypeQueryClickable ETraceTypeQuery::TraceTypeQuery3

static constexpr float UpdateLocationTimerRate{0.016f};

ADP_PlayerController::ADP_PlayerController()
{
    bShowMouseCursor = true;
    bEnableMouseOverEvents = true;
}

void ADP_PlayerController::UpdateGameState(EGameState NewGameState)
{
    CurrentGameState = NewGameState;
    UpdateInputMappingContext();
}

void ADP_PlayerController::UpdatePlayerLocation(const FVector& NewLocation)
{
    TargetPlayerLocation = NewLocation;
    GetWorldTimerManager().SetTimer(UpdatePlayerLocationTimerHandle, this, &ThisClass::OnUpdatePlayerLocationHandler, UpdateLocationTimerRate, true);
}

void ADP_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (CurrentGameState == EGameState::Placement)
    {
        if (FHitResult HitResult; GetHitResultUnderCursorByChannel(TraceTypeQueryNode, false, HitResult))
        {
            OnUpdatePreviewLocation.Broadcast(HitResult.GetActor());
        }
    }
}

void ADP_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(ClickAction);
    check(SelectAction);
    check(AnyKeyAction);
    check(GameInputMapping);
    check(WelcomeInputMapping);
    check(NoInputMapping);
    check(InspectInputMapping);

    SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
}

void ADP_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        Input->BindAction(ClickAction, ETriggerEvent::Started, this, &ThisClass::OnClickHandler);
        Input->BindAction(SelectAction, ETriggerEvent::Started, this, &ThisClass::OnSelectHandler);
        Input->BindAction(AnyKeyAction, ETriggerEvent::Started, this, &ThisClass::OnPressAnyKeyHandler);
    }
}

void ADP_PlayerController::UpdateInputMappingContext()
{
    if (const auto* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            InputSystem->RemoveMappingContext(CurrentInputMapping);

            switch (CurrentGameState)
            {
                case EGameState::Welcome:
                    CurrentInputMapping = WelcomeInputMapping;
                    break;
                case EGameState::Preload:
                    [[fallthrough]];
                case EGameState::Options:
                    [[fallthrough]];
                case EGameState::Warning:
                    [[fallthrough]];
                case EGameState::SaveAndLoad:
                    CurrentInputMapping = NoInputMapping;
                    break;
                case EGameState::Inspect:
                    CurrentInputMapping = InspectInputMapping;
                    break;
                default:
                    CurrentInputMapping = GameInputMapping;
                    break;
            }

            InputSystem->AddMappingContext(CurrentInputMapping, 0);
        }
    }
}

void ADP_PlayerController::ObjectPlacementClick()
{
    OnObjectSpawn.Broadcast();
}

void ADP_PlayerController::InteractClick()
{
    if (FHitResult HitResult; GetHitResultUnderCursorByChannel(TraceTypeQueryClickable, false, HitResult))
    {
        if (auto* PlaceableActor = Cast<ADP_PlaceableActor>(HitResult.GetActor()))
        {
            PlaceableActor->Interact(FTransform{HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint});
        }
    }
}

void ADP_PlayerController::ObjectPlacementSelect()
{
    OnDeselectPlacementObject.Broadcast();
}

void ADP_PlayerController::Select()
{
    if (FHitResult HitResult; GetHitResultUnderCursorByChannel(TraceTypeQueryClickable, false, HitResult))
    {
        OnObjectSelected.Broadcast(HitResult.GetActor());
    }
    else
    {
        OnObjectSelected.Broadcast(nullptr);
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
    switch (CurrentGameState)
    {
        case EGameState::Placement:
            ObjectPlacementSelect();
            break;
        case EGameState::Interact:
            [[fallthrough]];
        case EGameState::Select:
            Select();
            break;
        default:
            break;
    }
}

void ADP_PlayerController::OnPressAnyKeyHandler()
{
    OnWelcomeScreenCompleted.Broadcast();
}

void ADP_PlayerController::OnUpdatePlayerLocationHandler()
{
    if (auto* PlayerPawn = GetPawn())
    {
        if (PlayerPawn->GetActorLocation().Equals(TargetPlayerLocation))
        {
            GetWorldTimerManager().ClearTimer(UpdatePlayerLocationTimerHandle);
        }
        else
        {
            PlayerPawn->SetActorLocation(FMath::VInterpTo(PlayerPawn->GetActorLocation(), TargetPlayerLocation, UpdateLocationTimerRate, UpdatePlayerLocationSpeed));
        }
    }
}
