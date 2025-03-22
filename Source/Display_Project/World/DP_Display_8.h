// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/DP_Display_1.h"
#include "DP_Display_8.generated.h"

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_Display_8 : public ADP_Display_1
{
    GENERATED_BODY()

public:
    ADP_Display_8();

    virtual void SetPreviewMode(bool bEnabled) override;
    virtual void UpdateMaterial(UMaterialInterface* Material) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> GlassMesh;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_2;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_3;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_4;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_5;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_6;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_7;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_8;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TArray<UMaterialInterface*> DefaultGlassMaterials;
};
