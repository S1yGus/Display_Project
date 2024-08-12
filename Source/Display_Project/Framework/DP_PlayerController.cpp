// Display_Project, all rights reserved.

#include "Framework/DP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/DP_InteractiveInterface.h"

ADP_PlayerController::ADP_PlayerController()
{
    bShowMouseCursor = true;
    bEnableMouseOverEvents = true;
}

void ADP_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameAndUI());
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

void ADP_PlayerController::OnClick()
{
    if (FHitResult HitResult; GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery3, false, HitResult))    // ECC_GameTraceChannel1 "Clickable"
    {
        if (auto* ClickableActor = Cast<IDP_InteractiveInterface>(HitResult.GetActor()))
        {
            ClickableActor->Interact(FTransform{HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint});
        }
    }
}

void ADP_PlayerController::OnSelect()
{
    // TODO
}
