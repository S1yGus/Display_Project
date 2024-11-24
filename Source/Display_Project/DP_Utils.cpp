// Display_Project, all rights reserved.

#include "DP_Utils.h"

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
        case EAttributeType::ButtonText:
            return NSLOCTEXT("UI", "ButtonText_Loc", "Button text");
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
        case EGameState::Welcome:
            return EWidgetType::Welcome;
        case EGameState::Standby:
            [[fallthrough]];
        case EGameState::Placement:
            [[fallthrough]];
        case EGameState::Interact:
            [[fallthrough]];
        case EGameState::Select:
            [[fallthrough]];
        case EGameState::Warning:
            return EWidgetType::Game;
        case EGameState::Inspect:
            return EWidgetType::Inspect;
        default:
            return EWidgetType::Game;
    }
}
