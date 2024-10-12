// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DP_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class DISPLAY_PROJECT_API ADP_Player : public APawn
{
    GENERATED_BODY()

public:
    ADP_Player();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ZoomAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RotationAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ResetRotationAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float MinArmLength{400.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float MaxArmLength{1400.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float ZoomStep{200.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Zoom")
    float ZoomSpeed{6.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Rotation")
    float RotationSpeed{1.6f};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Rotation")
    FRotator DefaultRotation{-90.0, 0.0, -90.0};

    UPROPERTY(EditDefaultsOnly, Category = "Input|Rotation")
    float RotationPitchThreshold{2.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    float UIOffset{440.0f};

    virtual void BeginPlay() override;

private:
    float TargetArmLength{0};
    FTimerHandle ZoomTimerHandle;

    void UpdateSpringArmOffset(FViewport* Viewport, uint32 Value = 0);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void OnZoomHandler(const FInputActionValue& Value);
    void OnZoomingHandler();
    void OnRotationHandler(const FInputActionValue& Value);
    void OnResetRotationHandler();
};
