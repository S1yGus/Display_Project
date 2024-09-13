// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_CoreTypes.h"
#include "DP_Grid.generated.h"

class ADP_PlaceableActor;
class ADP_Node;
class ADP_Panel;
class ADP_GameModeBase;
class UMaterialInterface;

UCLASS()
class DISPLAY_PROJECT_API ADP_Grid : public AActor
{
    GENERATED_BODY()

public:
    ADP_Grid();

    void SpawnCurrentObject();
    void UpdatePreviewLocation(ADP_Node* ReferenceNode);

protected:
    UPROPERTY(EditAnywhere, Category = "Settings", Meta = (ClampMin = "0"))
    FIntPoint GridSize;

    UPROPERTY(EditAnywhere, Category = "Settings", Meta = (ClampMin = "0.0", Units = "cm"))
    FVector NodeSize;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<ADP_Node> NodeClass;

    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<ADP_Panel> PanelClass;

    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    TObjectPtr<UMaterialInterface> ValidPreviewMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    TObjectPtr<UMaterialInterface> InvalidPreviewMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Preview", Meta = (ClampMin = "0.0"))
    FVector MinPreviewScale{1.01f};

    UPROPERTY(EditDefaultsOnly, Category = "Preview", Meta = (ClampMin = "0.0"))
    FVector MaxPreviewScale{1.2f};

    UPROPERTY(EditDefaultsOnly, Category = "Preview", Meta = (ClampMin = "0.0"))
    float ScaleInterpSpeed{14.0f};

    virtual void BeginPlay() override;

private:
    EObjectType CurrentObjectType{EObjectType::None};
    FAttributesDataMap CurrentObjectAttributesMap;
    UPROPERTY()
    TObjectPtr<ADP_Node> SelectedNode;
    UPROPERTY()
    TObjectPtr<ADP_Node> CurrentValidNode;
    UPROPERTY()
    TSet<ADP_Node*> OccupiedNodes;
    UPROPERTY()
    TObjectPtr<ADP_PlaceableActor> PreviewObject;
    FTimerHandle SpawnTimerHandle;
    FVector TargetPreviewScale;
    bool bIsValidArea{true};
    bool bIsAreaVacant{true};
    bool bIsSpawning{false};

    FORCEINLINE [[nodiscard]] ADP_GameModeBase* GetGameMode() const;

    void Init();
    void SpawnPanel();
    void Free();
    FORCEINLINE [[nodiscard]] bool CanSpawn() const { return bIsValidArea && bIsAreaVacant; }
    bool SpawnPreview(const FTransform& SpawnTransform);
    void UpdatePreviewMaterial();
    [[nodiscard]] TOptional<ADP_Node*> GetValidPreviewNode(ADP_Node* Node, const FIntPoint& ObjectSize);
    FORCEINLINE [[nodiscard]] ADP_Node* GetValidPreviewNodeX(ADP_Node* Node, int32 ObjectSizeX);
    FORCEINLINE [[nodiscard]] ADP_Node* GetValidPreviewNodeY(ADP_Node* Node, int32 ObjectSizeY);
    bool ForEachNodeInArea(ADP_Node* StartNode, const FIntPoint& ObjectSize, TSet<ADP_Node*>& OutTraversedNodes, TFunction<bool(ADP_Node*)>&& Func);
    [[nodiscard]] bool IsAreaVacant(ADP_Node* StartNode, const FIntPoint& ObjectSize);
    bool Occupy(ADP_Node* StartNode, ADP_PlaceableActor* Object, TSet<ADP_Node*>& OutTraversedNodes);

    void OnSpawning();
    FORCEINLINE void SpawnObject();

    void OnGameStateChanged(EGameState NewGameState);
    void OnObjectTypeChanged(EObjectType NewObjectType);
    void OnAttributeChanged(EAttributeType AttributeType, FAttributeData AttributeData);
};
