// Display_Project, all rights reserved.

#include "DP_Utils.h"
#include "Kismet/GameplayStatics.h"
#include "World/DP_PlaceableActor.h"

AActor* DP::GetPlaceableActorByGuid(const UObject* WorldContextObject, const FGuid& Guid)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ADP_PlaceableActor::StaticClass(), FoundActors);
    auto** FoundActor = FoundActors.FindByPredicate(
        [&Guid](auto* Actor)
        {
            if (auto* PlaceableActor = Cast<ADP_PlaceableActor>(Actor))
            {
                return PlaceableActor->GetObjectGuid() == Guid;
            }
            return false;
        });
    return FoundActor ? *FoundActor : nullptr;
}

uint64 UI::UniqueID()
{
    static uint64 ID{0};
    return ID++;
}

FText UI::GenerateUniqueObjectName(const FText& Name)
{
    const FText Format = NSLOCTEXT("UI", "ObjectNameFormat_Loc", "{0}_{1}");
    return FText::FormatOrdered(Format, Name, LOCGEN_NUMBER_UNGROUPED(UI::UniqueID(), ""));
}

FText UI::GetAdjustedDefaultText(EAttributeType Type, const FText& Text)
{
    return Type == EAttributeType::ObjectName ? UI::GenerateUniqueObjectName(Text) : Text;
}

FText UI::ObjectTypeToText(EObjectType Type)
{
    switch (Type)
    {
        case EObjectType::Display:
            return NSLOCTEXT("UI", "Dispaly_Loc", "Display");
        case EObjectType::Button:
            return NSLOCTEXT("UI", "Button_Loc", "Button");
        default:
            return NSLOCTEXT("UI", "Unknown_Loc", "Unknown");
    }
}

FText UI::AttributeTypeToText(EAttributeType Type)
{
    switch (Type)
    {
        case EAttributeType::ObjectName:
            return NSLOCTEXT("UI", "ObjectName_Loc", "Name");
        case EAttributeType::DisplayText:
            return NSLOCTEXT("UI", "DisplayText_Loc", "Display text");
        case EAttributeType::ButtonLabel:
            return NSLOCTEXT("UI", "ButtonLabel_Loc", "Button label");
        case EAttributeType::Display:
            return NSLOCTEXT("UI", "Dispaly_Loc", "Display");
        default:
            return NSLOCTEXT("UI", "Unknown_Loc", "Unknown");
    }
}

EWidgetType UI::GameStateToWidgetType(EGameState GameState)
{
    switch (GameState)
    {
        case EGameState::Preload:
            return EWidgetType::Preload;
        case EGameState::Welcome:
            return EWidgetType::Welcome;
        case EGameState::Standby:
            [[fallthrough]];
        case EGameState::Placement:
            [[fallthrough]];
        case EGameState::Interact:
            [[fallthrough]];
        case EGameState::Select:
            return EWidgetType::Game;
        case EGameState::Inspect:
            return EWidgetType::Inspect;
        case EGameState::Options:
            return EWidgetType::Options;
        case EGameState::SaveAndLoad:
            return EWidgetType::SaveAndLoad;
        default:
            return EWidgetType::None;
    }
}
