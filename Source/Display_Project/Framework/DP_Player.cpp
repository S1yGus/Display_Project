// Display_Project, all rights reserved.

#include "Framework/DP_Player.h"
#include "Framework/DP_GameUserSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "World/DP_PlaceableActor.h"
#include "Kismet/KismetMathLibrary.h"

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
    CameraComponent->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
    CameraComponent->bOverrideAspectRatioAxisConstraint = true;
    CameraComponent->SetupAttachment(SpringArmComponent);

    InspectionPoint = CreateDefaultSubobject<USceneComponent>("InspectionPoint");
    check(InspectionPoint);
    InspectionPoint->SetupAttachment(CameraComponent);

    SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("InspectionLighting");
    check(SpotLightComponent);
    SpotLightComponent->SetVisibility(false);
    SpotLightComponent->SetupAttachment(CameraComponent);
}

void ADP_Player::StartInspect(TSubclassOf<ADP_PlaceableActor> Class, const FAttributesMap& Attributes)
{
    SpotLightComponent->SetVisibility(true);
    InspectionPoint->SetRelativeRotation(InspectionPointDefaultRotation);

    // Spawn
    if (InspectedObject)
    {
        InspectedObject->Destroy();
    }
    InspectedObject = GetWorld() ? GetWorld()->SpawnActor<ADP_PlaceableActor>(Class) : nullptr;
    check(InspectedObject);
    InspectedObject->Init(Attributes, FGuid(0, 0, 0, 0));
    InspectedObject->AttachToComponent(InspectionPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    // Scale
    InspectedObject->SetActorScale3D(FVector::ZeroVector);
    ScaleInspectedObject(FVector::OneVector);

    // Zoom
    PrevTargetArmLength = SpringArmComponent->TargetArmLength;
    SetSpringArmLength(MaxArmLength);

    // DOF
    CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = TargetFocalDistance;
    ChangeDOFSensorWidth(DOFSensorWidth);
}

void ADP_Player::StopInspect()
{
    SpotLightComponent->SetVisibility(false);

    // Scale
    ScaleInspectedObject(FVector::ZeroVector,
                         [this]()
                         {
                             InspectedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                             InspectedObject->Destroy();
                         });

    // Zoom
    SetSpringArmLength(PrevTargetArmLength);

    // DOF
    ChangeDOFSensorWidth(0.0f,
                         [this]()
                         {
                             CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = 0.0f;
                         });
}

void ADP_Player::BeginPlay()
{
    Super::BeginPlay();

    check(ZoomAction);
    check(RotationAction);
    check(ResetRotationAction);
    check(InspectedObjectRotationAction);

    TargetArmLength = SpringArmComponent->TargetArmLength;
    TargetFocalDistance = CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance;
    CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = 0.0f;

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
        Input->BindAction(InspectedObjectRotationAction, ETriggerEvent::Triggered, this, &ThisClass::OnInspectedObjectRotationHandler);
    }
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

void ADP_Player::ScaleInspectedObject(FVector TargetScale, FScaleComplete&& OnComplete)
{
    InspectedObjectTargetScale = TargetScale;
    if (!GetWorldTimerManager().IsTimerActive(InspectedObjectScaleTimerHandle))
    {
        GetWorldTimerManager().SetTimer(InspectedObjectScaleTimerHandle, this, &ThisClass::OnInspectedObjectScalingHandler, TimerRate, true);
    }
    OnScaleComplete = MoveTemp(OnComplete);
}

void ADP_Player::SetSpringArmLength(float TargetLength)
{
    TargetArmLength = TargetLength;
    if (!GetWorldTimerManager().IsTimerActive(ZoomTimerHandle))
    {
        GetWorldTimerManager().SetTimer(ZoomTimerHandle, this, &ThisClass::OnZoomingHandler, TimerRate, true);
    }
}

void ADP_Player::ChangeDOFSensorWidth(float TargetSensorWidth, FDOFSensorWidthChangeComplete&& OnComplete)
{
    TargetDOFSensorWidth = TargetSensorWidth;
    if (!GetWorldTimerManager().IsTimerActive(DOFSensorWidthChangeTimerHandle))
    {
        GetWorldTimerManager().SetTimer(DOFSensorWidthChangeTimerHandle, this, &ThisClass::OnDOFSensorWidthChangingHandler, TimerRate, true);
    }
    OnDOFSensorWidthChangeComplete = MoveTemp(OnComplete);
}

void ADP_Player::OnZoomHandler(const FInputActionValue& Value)
{
    const auto FloatValue = Value.Get<float>();

    SetSpringArmLength(FMath::Clamp(TargetArmLength + ZoomStep * FloatValue, MinArmLength, MaxArmLength));
}

void ADP_Player::OnZoomingHandler()
{
    if (FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, TargetArmLength, UE_KINDA_SMALL_NUMBER))
    {
        GetWorldTimerManager().ClearTimer(ZoomTimerHandle);
    }
    else
    {
        SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, TargetArmLength, TimerRate, ZoomingSpeed);
        if (GEngine && GEngine->GameViewport)
        {
            UpdateSpringArmOffset(GEngine->GameViewport->Viewport);
        }
    }
}

void ADP_Player::OnRotationHandler(const FInputActionValue& Value)
{
    const auto AxisValue = Value.Get<FVector2D>();

    if (auto* GameUserSettings = UDP_GameUserSettings::Get())
    {
        const auto RotationSpeed = GameUserSettings->GetRotationSpeed();
        if (SpringArmComponent->GetRelativeRotation().Pitch < RotationPitchThreshold || AxisValue.Y < 0.0)
        {
            SpringArmComponent->AddLocalRotation(FRotator{AxisValue.Y * RotationSpeed, 0.0, 0.0});
            if (SpringArmComponent->GetRelativeRotation().Pitch > RotationPitchThreshold)
            {
                const FRotator RotationThreshold{RotationPitchThreshold, SpringArmComponent->GetRelativeRotation().Yaw, SpringArmComponent->GetRelativeRotation().Roll};
                SpringArmComponent->SetRelativeRotation(RotationThreshold);
            }
        }
        SpringArmComponent->AddWorldRotation(FRotator{0.0, AxisValue.X * RotationSpeed, 0.0});
    }
}

void ADP_Player::OnResetRotationHandler()
{
    SpringArmComponent->SetWorldRotation(DefaultRotation);
}

void ADP_Player::OnInspectedObjectRotationHandler(const FInputActionValue& Value)
{
    const FVector2D AxisValue = Value.Get<FVector2D>();

    if (auto* GameUserSettings = UDP_GameUserSettings::Get())
    {
        const auto RotationSpeed = GameUserSettings->GetRotationSpeed();
        InspectionPoint->AddRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(AxisValue.X * RotationSpeed)));
        InspectionPoint->AddRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(AxisValue.Y * RotationSpeed)));
    }
}

void ADP_Player::OnInspectedObjectScalingHandler()
{
    if (InspectedObject->GetActorScale3D().Equals(InspectedObjectTargetScale, UE_KINDA_SMALL_NUMBER))
    {
        GetWorldTimerManager().ClearTimer(InspectedObjectScaleTimerHandle);

        if (OnScaleComplete)
        {
            OnScaleComplete();
            OnScaleComplete = nullptr;
        }
    }
    else
    {
        InspectedObject->SetActorScale3D(FMath::VInterpConstantTo(InspectedObject->GetActorScale3D(),    //
                                                                  InspectedObjectTargetScale,            //
                                                                  TimerRate,                             //
                                                                  InspectedObjectScalingSpeed));
    }
}

void ADP_Player::OnDOFSensorWidthChangingHandler()
{
    auto& DepthOfFieldSensorWidth = CameraComponent->PostProcessSettings.DepthOfFieldSensorWidth;
    if (FMath::IsNearlyEqual(DepthOfFieldSensorWidth, TargetDOFSensorWidth, UE_KINDA_SMALL_NUMBER))
    {
        GetWorldTimerManager().ClearTimer(DOFSensorWidthChangeTimerHandle);

        if (OnDOFSensorWidthChangeComplete)
        {
            OnDOFSensorWidthChangeComplete();
            OnDOFSensorWidthChangeComplete = nullptr;
        }
    }
    else
    {
        DepthOfFieldSensorWidth = FMath::FInterpTo(DepthOfFieldSensorWidth, TargetDOFSensorWidth, TimerRate, DOFSensorWidthChangingSpeed);
    }
}
