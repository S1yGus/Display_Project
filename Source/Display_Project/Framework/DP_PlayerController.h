// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DP_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class DISPLAY_PROJECT_API ADP_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADP_PlayerController();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> ClickAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> SelectAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnClick();
    void OnSelect();
};
