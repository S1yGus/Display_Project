// Display_Project, all rights reserved.

#include "Components/DP_FXComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

UDP_FXComponent::UDP_FXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDP_FXComponent::MakeInteractFX(const FTransform& InteractionTransform)
{
    if (InteractSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractSound, InteractionTransform.GetLocation());
    }

    if (InteractNiagaraSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,                                  //
                                                       InteractNiagaraSystem,                 //
                                                       InteractionTransform.GetLocation(),    //
                                                       InteractionTransform.Rotator());
    }
}
