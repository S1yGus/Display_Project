// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DP_SegmentInterface.h"
#include "DP_Segment.generated.h"

class UWidgetComponent;
class UDP_TextWidget;

UCLASS()
class DISPLAY_PROJECT_API ADP_Segment : public AActor, public IDP_SegmentInterface
{
    GENERATED_BODY()

public:
    ADP_Segment();

    virtual void Set(TCHAR Character) override;
    virtual void Clear() override;
    virtual void Teardown() override;

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UWidgetComponent> WidgetComponent;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<UDP_TextWidget> TextWidget;
};
