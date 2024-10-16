// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_TextShifter.generated.h"

class UTextRenderComponent;

UCLASS()
class DISPLAY_PROJECT_API ADP_TextShifter : public AActor
{
    GENERATED_BODY()

public:
    ADP_TextShifter();

    void InitShift();

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> TextRender;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FText ShiftText;

    UPROPERTY(EditAnywhere, Category = "Settings")
    float ShiftDelay{0.0f};

private:
    void OnShiftHandler();
};
