// Display_Project, all rights reserved.

#include "Components/DP_FXComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

UDP_FXComponent::UDP_FXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<USoundCue> ButtonClick_Cue(TEXT("/Game/Sounds/World/ButtonClick_Cue"));
    if (ButtonClick_Cue.Object)
    {
        InteractSound = ButtonClick_Cue.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_Click(TEXT("/Game/VFX/NS_Click"));
    if (NS_Click.Object)
    {
        InteractNiagaraSystem = NS_Click.Object;
    }
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
