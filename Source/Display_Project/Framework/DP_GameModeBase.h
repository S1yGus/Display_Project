// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DP_CoreTypes.h"
#include "DP_GameModeBase.generated.h"

class ADP_PlaceableActor;
class UMaterialInterface;

UCLASS()
class DISPLAY_PROJECT_API ADP_GameModeBase : public AGameMode
{
    GENERATED_BODY()

public:
    FOnGameStateChangedSignature OnGameStateChanged;

    void SpawnCurrentObject();

    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    TObjectPtr<UMaterialInterface> ValidPreviewMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    TObjectPtr<UMaterialInterface> InvalidPreviewMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    FVector MinPreviewScale{1.01f};

    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    FVector MaxPreviewScale{1.1f};

    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    float ScaleInterpSpeed{14.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TMap<EObjectType, TSubclassOf<ADP_PlaceableActor>> ObjectClasses;

private:
    EGameState CurrentGameState{EGameState::WaitingToStart};
    EObjectType CurrentObjectType{EObjectType::Button};
    UPROPERTY()
    TObjectPtr<ADP_PlaceableActor> PreviewObject;
    int32 OverlapCounter{0};
    FTimerHandle SpawnTimerHandle;
    FVector TargetScale;

    bool CanSpawn() { return OverlapCounter == 0; }
    void UpdatePreview();

    UFUNCTION()
    void OnPreviewBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnPreviewEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

    void OnSpawning();

    virtual void SetGameState(EGameState NewGameState);
};
