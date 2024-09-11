// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DP_CoreTypes.h"
#include "DP_GameModeBase.generated.h"

class ADP_Grid;

UCLASS()
class DISPLAY_PROJECT_API ADP_GameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    FOnGameStateChangedSignature OnGameStateChanged;
    FOnObjectTypeChangedSignature OnObjectTypeChanged;
    FOnAttributeChangedSignature OnAttributeChanged;

    [[nodiscard]] const TMap<EObjectType, FObjectData>& GetObjectsMap() const { return ObjectsMap; }

    void SetGameState(EGameState NewGameState);
    void SetCurrentObjectType(EObjectType NewObjectType);
    void AddCurrentObjectAttribute(EAttributeType AttributeType, FAttributeData AttributeData);

    void SpawnCurrentObject();
    void UpdatePreviewLocation(AActor* ReferenceActor);

    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TMap<EObjectType, FObjectData> ObjectsMap;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<ADP_Grid> GridClass;

private:
    EGameState CurrentGameState{EGameState::Standby};
    EObjectType CurrentObjectType{EObjectType::None};
    UPROPERTY()
    TObjectPtr<ADP_Grid> Grid;

    void SpawnGrid();

    void SetGameState_Internal(EGameState NewGameState);
    void SetCurrentObjectType_Internal(EObjectType NewObjectType);
};
