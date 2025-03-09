// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_Node.generated.h"

class UBoxComponent;

UCLASS()
class DISPLAY_PROJECT_API ADP_Node : public AActor
{
    GENERATED_BODY()

public:
    ADP_Node();

    void Occupy(const FGuid& Guid);
    void Free();
    [[nodiscard]] bool IsOccupied() const;
    void UpdateCollisionScale(const FVector& CollisionSize);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    TObjectPtr<UBoxComponent> BoxCollision;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Placement")
    void UpdateOccupiedState(bool bOccupiedState);

private:
    FGuid OccupyingGuid;
    UPROPERTY()
    TObjectPtr<ADP_Node> LeftNode;
    UPROPERTY()
    TObjectPtr<ADP_Node> RightNode;
    UPROPERTY()
    TObjectPtr<ADP_Node> TopNode;
    UPROPERTY()
    TObjectPtr<ADP_Node> BottomNode;

    friend class ADP_Grid;
};
