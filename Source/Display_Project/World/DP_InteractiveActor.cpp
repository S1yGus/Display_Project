// Display_Project, all rights reserved.

#include "World/DP_InteractiveActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ADP_InteractiveActor::ADP_InteractiveActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADP_InteractiveActor::Interact(const FTransform& InteractionTransform)
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

void ADP_InteractiveActor::BeginPlay()
{
    Super::BeginPlay();

    OnBeginCursorOver.AddDynamic(this, &ThisClass::OnBeginCursorHover);
    OnEndCursorOver.AddDynamic(this, &ThisClass::OnEndCursorHover);
}
