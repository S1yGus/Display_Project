// Display_Project, all rights reserved.

#include "Framework/DP_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Blueprint/WidgetLayoutLibrary.h"

constexpr static float TimerRate{0.016f};

static double HalfFOVTan(double FOV)
{
    return FMath::Tan(FMath::DegreesToRadians(FOV * 0.5));
}

ADP_Player::ADP_Player()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    check(SpringArmComponent);
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(CameraComponent);
    CameraComponent->bOverrideAspectRatioAxisConstraint = true;
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ADP_Player::BeginPlay()
{
    Super::BeginPlay();

    check(ZoomAction);
    check(RotationAction);
    check(ResetRotationAction);

    TargetArmLength = SpringArmComponent->TargetArmLength;

    if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
    {
        FViewport* Viewport = GEngine->GameViewport->Viewport;
        Viewport->ViewportResizedEvent.AddUObject(this, &ThisClass::UpdateSpringArmOffset);
        UpdateSpringArmOffset(Viewport);
    }
}

void ADP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::OnZoomHandler);
        Input->BindAction(RotationAction, ETriggerEvent::Triggered, this, &ThisClass::OnRotationHandler);
        Input->BindAction(ResetRotationAction, ETriggerEvent::Started, this, &ThisClass::OnResetRotationHandler);
    }
}

void ADP_Player::OnZoomHandler(const FInputActionValue& Value)
{
    const auto FloatValue = Value.Get<float>();
    TargetArmLength = FMath::Clamp(TargetArmLength + ZoomStep * FloatValue, MinArmLength, MaxArmLength);
    if (!GetWorldTimerManager().IsTimerActive(ZoomTimerHandle))
    {
        GetWorldTimerManager().SetTimer(ZoomTimerHandle, this, &ThisClass::OnZoomingHandler, TimerRate, true);
    }
}

void ADP_Player::OnZoomingHandler()
{
    if (FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, TargetArmLength, UE_KINDA_SMALL_NUMBER))
    {
        GetWorldTimerManager().ClearTimer(ZoomTimerHandle);
    }
    else
    {
        SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, TargetArmLength, TimerRate, ZoomSpeed);
        if (GEngine && GEngine->GameViewport)
        {
            UpdateSpringArmOffset(GEngine->GameViewport->Viewport);
        }
    }
}

void ADP_Player::OnRotationHandler(const FInputActionValue& Value)
{
    const auto AxisValue = Value.Get<FVector2D>();
    if (SpringArmComponent->GetRelativeRotation().Pitch < RotationPitchThreshold || AxisValue.Y < 0.0)
    {
        SpringArmComponent->AddLocalRotation(FRotator{AxisValue.Y * RotationSpeed, 0.0, 0.0});
    }
    SpringArmComponent->AddWorldRotation(FRotator{0.0, AxisValue.X * RotationSpeed, 0.0});
}

void ADP_Player::OnResetRotationHandler()
{
    SpringArmComponent->SetWorldRotation(DefaultRotation);
}

void ADP_Player::UpdateSpringArmOffset(FViewport* Viewport, uint32 Value)
{
    if (Viewport && Viewport->GetSizeXY().X)
    {
        const float UIOffsetFactor = UIOffset / Viewport->GetSizeXY().X * UWidgetLayoutLibrary::GetViewportScale(GetWorld());
        const FVector NewSocketOffset = {0.0,                                                                                                //
                                         SpringArmComponent->TargetArmLength * UIOffsetFactor * HalfFOVTan(CameraComponent->FieldOfView),    //
                                         0.0};
        SpringArmComponent->SocketOffset = NewSocketOffset;
    }
}
