// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_PlaceableActor.generated.h"

class UMaterialInterface;

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_PlaceableActor : public AActor
{
    GENERATED_BODY()

public:
    ADP_PlaceableActor();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview")
    void UpdatePreview(UMaterialInterface* PreviewMaterial);

protected:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Outline")
    void OnBeginCursorHover(AActor* TouchedActor);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Outline")
    void OnEndCursorHover(AActor* TouchedActor);

    virtual void BeginPlay() override;
};
