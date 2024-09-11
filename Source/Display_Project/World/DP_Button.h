// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/DP_PlaceableActor.h"
#include "Interfaces/DP_InteractiveInterface.h"
#include "DP_CoreTypes.h"
#include "DP_Button.generated.h"

class ADP_Display_1;

UCLASS()
class DISPLAY_PROJECT_API ADP_Button : public ADP_PlaceableActor, public IDP_InteractiveInterface
{
    GENERATED_BODY()

public:
    ADP_Button();

    virtual void Init(FAttributesDataMap&& Attributes) override;
    virtual void Interact(const FTransform& InteractionTransform) override;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Settings")
    void SetButtonText(const FString& Text);

protected:
    UPROPERTY(EditAnywhere, Category = "Settings")
    FString ButtonText;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString DisplayText;

    UPROPERTY(EditAnywhere, Category = "Settings")
    TObjectPtr<ADP_Display_1> Display;

    UPROPERTY(BlueprintReadWrite)
    bool bIsInteracting{false};

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interact")
    void OnInteract();

    virtual void BeginPlay() override;
};
