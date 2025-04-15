// Display_Project, all rights reserved.

#include "World/DP_Button.h"
#include "World/DP_Display.h"
#include "UI/DP_TextWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "DP_Utils.h"

DEFINE_LOG_CATEGORY_STATIC(LogButton, All, All)

static constexpr float AnimationTimerRate{0.016f};

ADP_Button::ADP_Button()
{
    PrimaryActorTick.bCanEverTick = false;
    Type = EObjectType::Button;

    // MeshComponent->Rename(TEXT("ButtonMesh"));

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ButtonText");
    check(WidgetComponent);
    WidgetComponent->SetCastShadow(false);
    WidgetComponent->SetupAttachment(MeshComponent);
}

void ADP_Button::Interact(const FTransform& InteractionTransform)
{
    Super::Interact(InteractionTransform);

    if (!bIsInteracting)
    {
        bIsInteracting = true;
        UpdateLinkedDisplayText();
        AnimateButton();
    }
}

void ADP_Button::BeginPlay()
{
    Super::BeginPlay();

    UpdateLabel(Label);

    OriginalMeshRelativeLocation = MeshComponent->GetRelativeLocation();
}

void ADP_Button::UpdateAttributes()
{
    Super::UpdateAttributes();

    if (AttributesMap.Contains(EAttributeType::DisplayText))
    {
        LinkedDisplayText = AttributesMap[EAttributeType::DisplayText].Get<FString>();
    }
    if (AttributesMap.Contains(EAttributeType::ButtonLabel))
    {
        Label = AttributesMap[EAttributeType::ButtonLabel].Get<FString>();
        UpdateLabel(Label);
    }
    if (AttributesMap.Contains(EAttributeType::Display))
    {
        LinkedDisplay = DP::GetPlaceableActorByGuid<ADP_Display>(GetWorld(), AttributesMap[EAttributeType::Display].Get<FGuid>());
    }
}

void ADP_Button::UpdateLabel(const FString& NewLabel)
{
    if (auto* TextWidget = Cast<UDP_TextWidget>(WidgetComponent->GetWidget()))
    {
        const auto NewButtonLabel = UKismetStringLibrary::GetSubstring(NewLabel, 0, MaxLabelLength);
        TextWidget->SetText(FText::FromString(NewButtonLabel));
    }
}

void ADP_Button::UpdateLinkedDisplayText()
{
    if (!IsValid(LinkedDisplay) && AttributesMap.Contains(EAttributeType::Display))
    {
        LinkedDisplay = DP::GetPlaceableActorByGuid<ADP_Display>(GetWorld(), AttributesMap[EAttributeType::Display].Get<FGuid>());
    }

    if (IsValid(LinkedDisplay))
    {
        TAttributeData Data;
        Data.Set<FString>(LinkedDisplayText);
        LinkedDisplay->UpdateAttribute(EAttributeType::DisplayText, Data);
    }
}

void ADP_Button::AnimateButton()
{
    TargetMeshRelativeLocation = OriginalMeshRelativeLocation + AnimationAmplitude;
    GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &ThisClass::OnButtonAnimation, AnimationTimerRate, true);
}

void ADP_Button::OnButtonAnimation()
{
    MeshComponent->SetRelativeLocation(FMath::VInterpTo(MeshComponent->GetRelativeLocation(), TargetMeshRelativeLocation, AnimationTimerRate, AnimationSpeed));

    if (MeshComponent->GetRelativeLocation().Equals(TargetMeshRelativeLocation))
    {
        if (TargetMeshRelativeLocation == OriginalMeshRelativeLocation)
        {
            GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
            MeshComponent->SetRelativeLocation(OriginalMeshRelativeLocation);
            bIsInteracting = false;
        }

        TargetMeshRelativeLocation = OriginalMeshRelativeLocation;
    }
}
