// Display_Project, all rights reserved.

#include "World/DP_Grid.h"
#include "World/DP_PlaceableActor.h"
#include "World/DP_Node.h"
#include "Framework/DP_GameModeBase.h"
#include "DP_Utils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All)

static constexpr float SpawnTimerRate{0.016f};

FORCEINLINE static bool IsEven(int32 Position)
{
    return Position % 2 == 0;
}

ADP_Grid::ADP_Grid()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADP_Grid::SpawnCurrentObject()
{
    if (IsValid(PreviewObject) && CanSpawn())
    {
        bIsSpawning = true;
        TargetPreviewScale = MaxPreviewScale;
        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawning, SpawnTimerRate, true);
    }
}

void ADP_Grid::UpdatePreviewLocation(ADP_Node* ReferenceNode)
{
    if (!ReferenceNode || ReferenceNode == SelectedNode || bIsSpawning)
        return;

    SelectedNode = ReferenceNode;

    if (!IsValid(PreviewObject) && !SpawnPreview(FTransform{ReferenceNode->GetActorRotation(), ReferenceNode->GetActorLocation(), MinPreviewScale}))
        return;

    const TOptional<ADP_Node*> ValidNodeOptional = GetValidPreviewNode(ReferenceNode, PreviewObject->GetObjectSize());
    bIsValidArea = ValidNodeOptional.IsSet();
    if (bIsValidArea)
    {
        CurrentValidNode = ValidNodeOptional.GetValue();
        PreviewObject->SetActorLocation(CurrentValidNode->GetActorLocation());
        bIsAreaVacant = IsAreaVacant(CurrentValidNode, PreviewObject->GetObjectSize());
    }

    UpdatePreviewMaterial();
}

void ADP_Grid::BeginPlay()
{
    Super::BeginPlay();

    if (auto* GameMode = GetGameMode())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
        GameMode->OnObjectTypeChanged.AddUObject(this, &ThisClass::OnObjectTypeChanged);
        GameMode->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
    }

    Init();
}

ADP_GameModeBase* ADP_Grid::GetGameMode() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ADP_GameModeBase>() : nullptr;
}

void ADP_Grid::Init()
{
    TArray<TObjectPtr<ADP_Node>> Nodes;
    Nodes.Reserve(GridSize.X * GridSize.Y);

    const auto StartSpawnLocation = FVector{-(GridSize.X - 1) * NodeSize.X / 2, -(GridSize.Y - 1) * NodeSize.Y / 2, 0.0};
    for (int32 y = 0; y < GridSize.Y; ++y)
    {
        for (int32 x = 0; x < GridSize.X; ++x)
        {
            const FTransform RelativeSpawnTransform{StartSpawnLocation + FVector{x * NodeSize.X, y * NodeSize.Y, 0.0}};
            auto* Node = GetWorld()->SpawnActor<ADP_Node>(NodeClass, RelativeSpawnTransform * GetActorTransform());
            check(Node);
            Node->UpdateCollisionScale(NodeSize);
            Nodes.Add(Node);
        }
    }

    for (int32 i = 0; i < Nodes.Num(); ++i)
    {
        // Left
        if (i % GridSize.X != 0)
        {
            Nodes[i]->LeftNode = Nodes[i - 1];
        }

        // Right
        if ((i + 1) % GridSize.X != 0)
        {
            Nodes[i]->RightNode = Nodes[i + 1];
        }

        // Top
        if (i > GridSize.X - 1)
        {
            Nodes[i]->TopNode = Nodes[i - GridSize.X];
        }

        // Bottom
        if (i < GridSize.X * GridSize.Y - GridSize.X)
        {
            Nodes[i]->BottomNode = Nodes[i + GridSize.X];
        }
    }
}

void ADP_Grid::Free()
{
    for (auto* Node : OccupiedNodes)
    {
        Node->Free();
    }

    OccupiedNodes.Empty();
}

bool ADP_Grid::SpawnPreview(const FTransform& SpawnTransform)
{
    if (auto* GameMode = GetGameMode())
    {
        if (const auto& ObjectsMap = GameMode->GetObjectsMap(); ObjectsMap.Contains(CurrentObjectType) && GetWorld())
        {
            PreviewObject = GetWorld()->SpawnActor<ADP_PlaceableActor>(ObjectsMap[CurrentObjectType].Class, SpawnTransform);
            check(PreviewObject);

            return true;
        }
    }

    return false;
}

void ADP_Grid::UpdatePreviewMaterial()
{
    PreviewObject->UpdatePreview(CanSpawn() ? ValidPreviewMaterial : InvalidPreviewMaterial);
}

TOptional<ADP_Node*> ADP_Grid::GetValidPreviewNode(ADP_Node* Node, const FIntPoint& ObjectSize)
{
    if (ObjectSize.X > GridSize.X || ObjectSize.Y > GridSize.Y)
    {
        UE_LOG(LogGrid, Warning, TEXT("The object's dimensions exceed the size of the grid!"));

        return NullOpt;
    }

    if (ObjectSize.X > 1)
    {
        Node = GetValidPreviewNodeX(Node, ObjectSize.X);
    }

    if (ObjectSize.Y > 1)
    {
        Node = GetValidPreviewNodeY(Node, ObjectSize.Y);
    }

    return Node;
}

ADP_Node* ADP_Grid::GetValidPreviewNodeX(ADP_Node* Node, int32 ObjectSizeX)
{
    // Left
    auto* NodeX = Node;
    for (int32 i = 0; i < ObjectSizeX / 2; ++i)
    {
        if (!NodeX->LeftNode)
        {
            for (int32 j = 0; j < ObjectSizeX / 2 - i; ++j)
            {
                Node = Node->RightNode;
            }
            return Node;
        }
        NodeX = NodeX->LeftNode;
    }

    // Right
    NodeX = Node;
    const int8 EvenAddition = IsEven(ObjectSizeX) ? -1 : 0;
    for (int32 i = 0; i < ObjectSizeX / 2 + EvenAddition; ++i)
    {
        if (!NodeX->RightNode)
        {
            for (int32 j = 0; j < ObjectSizeX / 2 - i + EvenAddition; ++j)
            {
                Node = Node->LeftNode;
            }
            return Node;
        }
        NodeX = NodeX->RightNode;
    }

    return Node;
}

ADP_Node* ADP_Grid::GetValidPreviewNodeY(ADP_Node* Node, int32 ObjectSizeY)
{
    // Top
    auto* NodeY = Node;
    for (int32 i = 0; i < ObjectSizeY / 2; ++i)
    {
        if (!NodeY->TopNode)
        {
            for (int32 j = 0; j < ObjectSizeY / 2 - i; ++j)
            {
                Node = Node->BottomNode;
            }
            return Node;
        }
        NodeY = NodeY->TopNode;
    }

    // Bottom
    NodeY = Node;
    const int8 EvenAddition = IsEven(ObjectSizeY) ? -1 : 0;
    for (int32 i = 0; i < ObjectSizeY / 2 + EvenAddition; ++i)
    {
        if (!NodeY->BottomNode)
        {
            for (int32 j = 0; j < ObjectSizeY / 2 - i + EvenAddition; ++j)
            {
                Node = Node->TopNode;
            }
            return Node;
        }
        NodeY = NodeY->BottomNode;
    }

    return Node;
}

bool ADP_Grid::ForEachNodeInArea(ADP_Node* StartNode, const FIntPoint& ObjectSize, TSet<ADP_Node*>& OutTraversedNodes, TFunction<bool(ADP_Node*)>&& Func)
{
    for (int32 y = 0; y < ObjectSize.Y / 2; ++y)
    {
        if (!StartNode)
            return false;

        StartNode = StartNode->TopNode;
    }

    for (int32 x = 0; x < ObjectSize.X / 2; ++x)
    {
        if (!StartNode)
            return false;

        StartNode = StartNode->LeftNode;
    }

    auto* TraversingNode = StartNode;
    int32 CurrentY = 0;
    while (true)
    {
        int32 CurrnetX = 0;
        while (true)
        {
            if (!Func(TraversingNode))
                return false;

            OutTraversedNodes.Add(TraversingNode);

            if (!TraversingNode || CurrnetX == ObjectSize.X - 1)
                break;

            TraversingNode = IsEven(CurrentY) ? TraversingNode->RightNode : TraversingNode->LeftNode;
            ++CurrnetX;
        }

        if (!TraversingNode || CurrentY == ObjectSize.Y - 1)
            break;

        TraversingNode = TraversingNode->BottomNode;
        ++CurrentY;
    }

    return true;
}

bool ADP_Grid::IsAreaVacant(ADP_Node* StartNode, const FIntPoint& ObjectSize)
{
    TSet<ADP_Node*> Placeholder;
    Placeholder.Reserve(ObjectSize.X * ObjectSize.Y);
    return ForEachNodeInArea(StartNode, ObjectSize, Placeholder,
                             [](const auto* Node)
                             {
                                 return !Node->IsOccupied();
                             });
}

bool ADP_Grid::Occupy(ADP_Node* StartNode, ADP_PlaceableActor* Object, TSet<ADP_Node*>& OutTraversedNodes)
{
    return ForEachNodeInArea(StartNode, Object->GetObjectSize(), OutTraversedNodes,
                             [Object](auto* Node)
                             {
                                 if (Node)
                                 {
                                     Node->Occupy(Object);
                                 }
                                 return true;
                             });
}

void ADP_Grid::OnSpawning()
{
    PreviewObject->SetActorScale3D(FMath::VInterpTo(PreviewObject->GetActorScale3D(), TargetPreviewScale, SpawnTimerRate, ScaleInterpSpeed));

    if (PreviewObject->GetActorScale3D().Equals(MaxPreviewScale))
    {
        TargetPreviewScale = MinPreviewScale;
    }
    else if (PreviewObject->GetActorScale3D().Equals(MinPreviewScale))
    {
        SpawnObject();
    }
}

void ADP_Grid::SpawnObject()
{
    if (auto* GameMode = GetGameMode())
    {
        if (const auto& ObjectsMap = GameMode->GetObjectsMap(); ObjectsMap.Contains(CurrentObjectType) && GetWorld())
        {
            GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
            const auto SpawnTransform = FTransform{PreviewObject->GetActorRotation(), PreviewObject->GetActorLocation()};
            auto* Object = GetWorld()->SpawnActorDeferred<ADP_PlaceableActor>(ObjectsMap[CurrentObjectType].Class, SpawnTransform);
            check(Object);
            Object->Init(MoveTemp(CurrentObjectAttributesMap));
            Object->FinishSpawning(SpawnTransform);

            TSet<ADP_Node*> TraversedNodes;
            const FIntPoint ObjectSize = Object->GetObjectSize();
            TraversedNodes.Reserve(ObjectSize.X * ObjectSize.Y);
            if (Occupy(CurrentValidNode, Object, TraversedNodes))
            {
                OccupiedNodes = OccupiedNodes.Union(TraversedNodes);
            }
            else
            {
                UE_LOG(LogGrid, Error, TEXT("Error placing object %s in the specified area!"), *Object->GetName());

                for (auto* Node : TraversedNodes)
                {
                    Node->Free();
                }

                if (IsValid(Object))
                {
                    Object->Destroy();
                }
            }

            GameMode->SetCurrentObjectType(EObjectType::None);
            GameMode->SetGameState(EGameState::Interact);
            bIsSpawning = false;
        }
        else
        {
            UE_LOG(LogGrid, Error, TEXT("Data for object type %s is not set!"), *DP::EnumToString(CurrentObjectType));
        }
    }
}

void ADP_Grid::OnGameStateChanged(EGameState NewGameState)
{
    switch (NewGameState)
    {
        case EGameState::Standby:
            Free();
            break;
        default:
            if (PreviewObject)
            {
                PreviewObject->Destroy();
            }
            break;
    }
}

void ADP_Grid::OnObjectTypeChanged(EObjectType NewObjectType)
{
    CurrentObjectType = NewObjectType;
    CurrentObjectAttributesMap.Empty();
    SelectedNode = nullptr;

    if (PreviewObject)
    {
        PreviewObject->Destroy();
    }
}

void ADP_Grid::OnAttributeChanged(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (!CurrentObjectAttributesMap.Contains(AttributeType))
    {
        CurrentObjectAttributesMap.Add(AttributeType);
    }

    CurrentObjectAttributesMap[AttributeType] = AttributeData;
}