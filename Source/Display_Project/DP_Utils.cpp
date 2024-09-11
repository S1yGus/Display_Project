// Display_Project, all rights reserved.

#include "DP_Utils.h"

uint64 UI::UniqueID()
{
    static uint64 ID{0};
    return ID++;
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

FText UI::GenerateUniqueObjectName(const FText& Name)
{
    FText Format = NSLOCTEXT("UI", "ObjectNameFormat_Loc", "{0}_{1}");
    return FText::FormatOrdered(Format, Name, LOCGEN_NUMBER_UNGROUPED(UniqueID(), ""));
}
