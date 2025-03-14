// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_Panel.generated.h"

UCLASS()
class DISPLAY_PROJECT_API ADP_Panel : public AActor
{
    GENERATED_BODY()

public:
    ADP_Panel();

    void SetLabel(const FText& NewLabel);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> PanelMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    FString ScrewSocketNamePattern{"Screw_"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    int32 ScrewNum{4};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    FText Label;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<AActor> ScrewClass;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Settings")
    void UpdatePanelLabel(const FText& NewLabel);

    virtual void BeginPlay() override;

private:
    void SpawnScrews();
};
