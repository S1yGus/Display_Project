// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DP_FXComponent.generated.h"

class USoundCue;
class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DISPLAY_PROJECT_API UDP_FXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDP_FXComponent();

    void MakeInteractFX(const FTransform& InteractionTransform);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "FX")
    TObjectPtr<USoundCue> InteractSound;

    UPROPERTY(EditDefaultsOnly, Category = "FX")
    TObjectPtr<UNiagaraSystem> InteractNiagaraSystem;
};
