// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/DP_PlaceableActor.h"
#include "DP_CoreTypes.h"
#include "DP_Display_1.generated.h"

#define CREATE_SEGMENT(Segment)                                    \
    Segment = CreateDefaultSubobject<UDP_Segment>(TEXT(#Segment)); \
    check(Segment);                                                \
    Segment->SetupAttachment(GetRootComponent());                  \
    Segments.Add(Segment);

class IDP_SegmentInterface;
class UDP_BaseScrollingAlgorithm;
class UDP_Segment;

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_Display_1 : public ADP_PlaceableActor
{
    GENERATED_BODY()

public:
    ADP_Display_1();

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_1;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString DisplayText;

    UPROPERTY(EditAnywhere, Category = "Classes")
    TSubclassOf<UDP_BaseScrollingAlgorithm> ScrollingAlgorithmClass;

    TArray<TWeakInterfacePtr<IDP_SegmentInterface>> Segments;

    virtual void BeginPlay() override;
    virtual void UpdateAttributes() override;

private:
    UPROPERTY()
    TObjectPtr<UDP_BaseScrollingAlgorithm> ScrollingAlgorithm;

    void RefreshText(const FString& Text);
};
