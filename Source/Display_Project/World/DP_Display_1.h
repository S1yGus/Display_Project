// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_Display_1.generated.h"

#define CREATE_SEGMENT(Segment)                                    \
    Segment = CreateDefaultSubobject<UDP_Segment>(TEXT(#Segment)); \
    check(Segment);                                                \
    Segment->SetupAttachment(GetRootComponent());                  \
    Segments.Add(Segment);                                         \
    Segments.Last(0)->Set(TEXT(#Segment)[FCString::Strlen(TEXT(#Segment)) - 1]);

class IDP_SegmentInterface;
class UDP_BaseScrollingAlgorithm;
class UDP_Segment;

UCLASS(Abstract)
class DISPLAY_PROJECT_API ADP_Display_1 : public AActor
{
    GENERATED_BODY()

public:
    ADP_Display_1();

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UDP_Segment> Segment_1;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString DisplayText;

    UPROPERTY(EditAnywhere, Category = "Settings")
    TSubclassOf<UDP_BaseScrollingAlgorithm> ScrollingAlgorithmClass;

    TArray<TWeakInterfacePtr<IDP_SegmentInterface>> Segments;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<UDP_BaseScrollingAlgorithm> ScrollingAlgorithm;
};