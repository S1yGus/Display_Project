// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "DP_CoreTypes.h"
#include "DP_GridController.generated.h"

class ADP_Grid;
class ATargetPoint;
class ADP_TextShifter;

UCLASS()
class DISPLAY_PROJECT_API ADP_GridController : public AInfo
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Classes")
    TMap<EObjectType, FObjectData> ObjectsMap;

    UPROPERTY(EditAnywhere, Category = "Classes")
    TSubclassOf<ADP_Grid> GridClass;

    UPROPERTY(EditAnywhere, Category = "Welcome")
    TObjectPtr<ATargetPoint> WelcomePoint;

    UPROPERTY(EditAnywhere, Category = "Welcome")
    TArray<TObjectPtr<ADP_TextShifter>> WelcomeText;

    virtual void BeginPlay() override;

private:
    EGameState CurrentGameState{EGameState::Standby};
    EObjectType CurrentObjectType{EObjectType::None};
    UPROPERTY()
    TObjectPtr<ADP_Grid> Grid;
    UPROPERTY()
    TObjectPtr<ADP_PlaceableActor> SelectedObject;

    void SpawnGrid();

    void SetGameState(EGameState NewGameState);
    void SetGameState_Internal(EGameState NewGameState);
    void SetCurrentObjectType(EObjectType NewObjectType);
    void SetCurrentObjectType_Internal(EObjectType NewObjectType);

    void OnStartGameHandler();
    void OnUpdatePreviewLocationHandler(AActor* ReferenceActor);
    void OnSpawnCurrentObjectHandler();
    void OnObjectSpawnedHandler();
    void OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData);
    void OnSelectHandler(AActor* SelectedActor, const FTransform& SelectionTransform);
    void OnDestroySelectedHandler();
    void OnDestroyAllHandler();
};
