// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DP_ClickableInterface.h"
#include "DP_Button.generated.h"

class ADP_Display_1;

UCLASS()
class DISPLAY_PROJECT_API ADP_Button : public AActor, public IDP_ClickableInterface
{
    GENERATED_BODY()

public:
    ADP_Button();

    virtual void Interact() override;

protected:
    UPROPERTY(EditAnywhere, Category = "Settings")
    FString Text;

    UPROPERTY(EditAnywhere, Category = "Settings")
    TObjectPtr<ADP_Display_1> Display;
};
