// Display_Project, all rights reserved.

#include "World/DP_Button.h"
#include "World/DP_Display_1.h"

DEFINE_LOG_CATEGORY_STATIC(LogButton, All, All)

ADP_Button::ADP_Button()
{
    PrimaryActorTick.bCanEverTick = false;
    Type = EObjectType::Button;
}

void ADP_Button::Interact(const FTransform& InteractionTransform)
{
    if (!bIsInteracting)
    {
        if (IsValid(Display))
        {
            FAttributeData Data;
            Data.Set<FString>(DisplayText);
            Display->UpdateAttribute(EAttributeType::DisplayText, Data);
        }
        OnInteract();
    }
}

void ADP_Button::BeginPlay()
{
    Super::BeginPlay();

    SetButtonText(ButtonText);
}

void ADP_Button::UpdateAttributes()
{
    Super::UpdateAttributes();

    if (AttributesMap.Contains(EAttributeType::DisplayText))
    {
        DisplayText = AttributesMap[EAttributeType::DisplayText].Get<FString>();
    }
    if (AttributesMap.Contains(EAttributeType::ButtonText))
    {
        ButtonText = AttributesMap[EAttributeType::ButtonText].Get<FString>();
        SetButtonText(ButtonText);
    }
    if (AttributesMap.Contains(EAttributeType::Display))
    {
        Display = AttributesMap[EAttributeType::Display].Get<TObjectPtr<ADP_Display_1>>();
    }
}
