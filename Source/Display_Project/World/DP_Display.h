// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/DP_PlaceableActor.h"
#include "DP_CoreTypes.h"
#include "DP_Display.generated.h"

class UDP_BaseScrollingAlgorithm;
class ADP_Segment;
class IDP_SegmentInterface;

UCLASS()
class DISPLAY_PROJECT_API ADP_Display : public ADP_PlaceableActor
{
    GENERATED_BODY()

public:
    ADP_Display();

    virtual void SetPreviewMode(bool bEnabled) override;
    virtual void UpdateMaterial(UMaterialInterface* Material) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> GlassMesh;

    UPROPERTY(EditAnywhere, Category = "Classes")
    TSubclassOf<ADP_Segment> SegmentClass;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    FString SegmentSocketNamePattern{"Segment_"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    int32 SegmentsNum{8};

    UPROPERTY(EditAnywhere, Category = "Classes")
    TSubclassOf<UDP_BaseScrollingAlgorithm> ScrollingAlgorithmClass;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString DisplayText;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void UpdateAttributes() override;

private:
    UPROPERTY()
    TObjectPtr<UDP_BaseScrollingAlgorithm> ScrollingAlgorithm;
    UPROPERTY()
    TArray<UMaterialInterface*> DefaultGlassMaterials;
    TArray<TWeakInterfacePtr<IDP_SegmentInterface>> Segments;

    FORCEINLINE void SpawnSegments();
    FORCEINLINE void CollectDefaultGlassMaterials();
    FORCEINLINE void CreateScrollingAlgorithm();
    void RefreshText(const FString& Text);
};
