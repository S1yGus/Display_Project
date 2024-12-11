// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DP_CoreAliases.h"
#include "DP_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class ADP_PlaceableActor;
struct FInputActionValue;

UCLASS()
class DISPLAY_PROJECT_API ADP_Player : public APawn
{
    GENERATED_BODY()

public:
    ADP_Player();

    void StartInspect(TSubclassOf<ADP_PlaceableActor> Class, const FAttributesMap& Attributes);
    void StopInspect();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USceneComponent> InspectionPoint;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ZoomAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RotationAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ResetRotationAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> InspectedObjectRotationAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float MinArmLength{400.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float MaxArmLength{1400.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float ZoomStep{200.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float ZoomingSpeed{6.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Rotation")
    FRotator DefaultRotation{-90.0, 0.0, -90.0};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Rotation")
    float RotationPitchThreshold{2.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    float UIOffset{440.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Inspect")
    FRotator InspectionPointDefaultRotation{0.0f, 90.0f, 90.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Inspect")
    float InspectedObjectScalingSpeed{5.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Inspect")
    float DOFSensorWidth{365.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Inspect")
    float DOFSensorWidthChangingSpeed{10.0f};

    virtual void BeginPlay() override;

private:
    FTimerHandle ZoomTimerHandle;
    float TargetArmLength{0.0f};
    float PrevTargetArmLength{0.0f};

    UPROPERTY()
    TObjectPtr<ADP_PlaceableActor> InspectedObject{nullptr};
    FTimerHandle InspectedObjectScaleTimerHandle;
    FVector InspectedObjectTargetScale{FVector::ZeroVector};
    FScaleComplete OnScaleComplete{nullptr};

    FTimerHandle DOFSensorWidthChangeTimerHandle;
    float TargetDOFSensorWidth{0.0f};
    float TargetFocalDistance{0.0f};
    FDOFSensorWidthChangeComplete OnDOFSensorWidthChangeComplete{nullptr};

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FORCEINLINE void UpdateSpringArmOffset(FViewport* Viewport, uint32 Value = 0);
    FORCEINLINE void ScaleInspectedObject(FVector TargetScale, FScaleComplete&& OnComplete = nullptr);
    FORCEINLINE void SetSpringArmLength(float TargetLength);
    FORCEINLINE void ChangeDOFSensorWidth(float TargetSensorWidth, FDOFSensorWidthChangeComplete&& OnComplete = nullptr);

    void OnZoomHandler(const FInputActionValue& Value);
    void OnZoomingHandler();
    void OnRotationHandler(const FInputActionValue& Value);
    void OnResetRotationHandler();
    void OnInspectedObjectRotationHandler(const FInputActionValue& Value);
    void OnInspectedObjectScalingHandler();
    void OnDOFSensorWidthChangingHandler();
};
