// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DP_InteractiveInterface.h"
#include "DP_InteractiveActor.generated.h"

class USoundCue;
class UNiagaraSystem;

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_InteractiveActor : public AActor, public IDP_InteractiveInterface
{
    GENERATED_BODY()

public:
    ADP_InteractiveActor();

    virtual void Interact(const FTransform& InteractionTransform) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "FX")
    TObjectPtr<USoundCue> InteractSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* InteractNiagaraSystem;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Outline")
    void OnBeginCursorHover(AActor* TouchedActor);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Outline")
    void OnEndCursorHover(AActor* TouchedActor);

    virtual void BeginPlay() override;
};
