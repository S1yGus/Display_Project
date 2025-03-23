// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_Panel.generated.h"

class UWidgetComponent;

UCLASS()
class DISPLAY_PROJECT_API ADP_Panel : public AActor
{
    GENERATED_BODY()

public:
    ADP_Panel();

    void SetLabel(const FString& NewLabel);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> PanelMesh;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UWidgetComponent> WidgetComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    FString Label;

    UPROPERTY(EditAnywhere, Category = "Settings", Meta = (ClampMin = "0"))
    int32 MaxLabelLength{42};

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<AActor> ScrewClass;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    FString ScrewSocketNamePattern{"Screw_"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    int32 ScrewsNum{4};

    virtual void BeginPlay() override;

private:
    void SpawnScrews();
    void UpdateLabel(const FString& NewLabel);
};
