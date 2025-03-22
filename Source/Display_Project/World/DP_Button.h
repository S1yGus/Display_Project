// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/DP_PlaceableActor.h"
#include "DP_CoreTypes.h"
#include "DP_Button.generated.h"

class ADP_Display_1;
class UWidgetComponent;

UCLASS()
class DISPLAY_PROJECT_API ADP_Button : public ADP_PlaceableActor
{
    GENERATED_BODY()

public:
    ADP_Button();

    virtual void Interact(const FTransform& InteractionTransform) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UWidgetComponent> WidgetComponent;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString Label;

    UPROPERTY(EditAnywhere, Category = "Settings", Meta = (ClampMin = "0"))
    int32 MaxLabelLength{3};

    UPROPERTY(EditAnywhere, Category = "Settings")
    TObjectPtr<ADP_Display_1> LinkedDisplay;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString LinkedDisplayText;

    UPROPERTY(EditAnywhere, Category = "Animation")
    FVector AnimationAmplitude{0.0, 0.0, -22.0};

    UPROPERTY(EditAnywhere, Category = "Animation", Meta = (ClampMin = "0.0"))
    float AnimationSpeed{14.0f};

    virtual void BeginPlay() override;
    virtual void UpdateAttributes() override;

private:
    FTimerHandle AnimationTimerHandle;
    FVector OriginalMeshRelativeLocation;
    FVector TargetMeshRelativeLocation;
    bool bIsInteracting{false};

    void UpdateLabel(const FString& NewLabel);
    void UpdateLinkedDisplayText();
    void AnimateButton();
    void OnButtonAnimation();
};
