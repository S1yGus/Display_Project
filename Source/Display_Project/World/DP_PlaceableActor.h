// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_CoreEnums.h"
#include "DP_CoreAliases.h"
#include "DP_PlaceableActor.generated.h"

class UMaterialInterface;

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_PlaceableActor : public AActor
{
    GENERATED_BODY()

public:
    ADP_PlaceableActor();

    [[nodiscard]] FString GetObjectName() const;
    [[nodiscard]] FIntPoint GetObjectSize() const { return Size; }

    virtual void Init(FAttributesDataMap&& Attributes);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview")
    void UpdatePreview(UMaterialInterface* PreviewMaterial);

protected:
    FAttributesDataMap AttributesMap;

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
    FIntPoint Size{1};
};
