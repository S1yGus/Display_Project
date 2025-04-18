// Display_Project, all rights reserved.

#include "UI/DP_AttributesListWidget.h"
#include "UI/DP_BaseAttributeWidget.h"
#include "UI/DP_TextInputAttributeWidget.h"
#include "UI/DP_ComboBoxAttributeWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "World/DP_Display.h"
#include "DP_Utils.h"

void UDP_AttributesListWidget::Init(EObjectType ObjectType, const TSet<EAttributeType>& Attributes)
{
    AttributesBox->ClearChildren();
    for (const auto AttributeType : Attributes)
    {
        AttributesBox->AddChild(CreateAttributeWidget(AttributeType, ObjectType));
    }
}

void UDP_AttributesListWidget::Reset()
{
    for (auto* Widget : AttributesBox->GetAllChildren())
    {
        if (auto* AttributeWidget = Cast<UDP_BaseAttributeWidget>(Widget))
        {
            AttributeWidget->Reset();
        }
    }
}

void UDP_AttributesListWidget::Update(const TAttributesMap& Attributes)
{
    for (auto* Widget : AttributesBox->GetAllChildren())
    {
        if (auto* AttributeWidget = Cast<UDP_BaseAttributeWidget>(Widget))
        {
            if (auto AttributeType = AttributeWidget->GetType();    //
                Attributes.Contains(AttributeType))
            {
                AttributeWidget->Update(Attributes[AttributeType]);
            }
            else
            {
                AttributeWidget->Reset();
            }
        }
    }
}

void UDP_AttributesListWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(AttributesBox);
}

TObjectPtr<UDP_BaseAttributeWidget> UDP_AttributesListWidget::CreateAttributeWidget(EAttributeType AttributeType, EObjectType ObjectType)
{
    TObjectPtr<UDP_BaseAttributeWidget> AttributeWidget;
    switch (AttributeType)
    {
        case EAttributeType::ObjectName:
            AttributeWidget = CreateTextInputAttributeWidget(UI::ObjectTypeToText(ObjectType));
            break;
        case EAttributeType::DisplayText:
            [[fallthrough]];
        case EAttributeType::ButtonLabel:
            AttributeWidget = CreateTextInputAttributeWidget(FText::GetEmpty());
            break;
        case EAttributeType::Display:
            AttributeWidget = CreateComboBoxAttributeWidget(
                [this](TObjectPtr<UComboBoxString> ComboBox, TArray<TAttributeData>& AttributeData)
                {
                    ComboBox->ClearOptions();
                    AttributeData.Empty();
                    TArray<AActor*> DisplayActors;
                    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADP_Display::StaticClass(), DisplayActors);
                    for (auto* Actor : DisplayActors)
                    {
                        if (auto* DisplayActor = Cast<ADP_Display>(Actor))
                        {
                            TAttributeData Data;
                            Data.Set<FGuid>(DisplayActor->GetObjectGuid());
                            AttributeData.Emplace(Data);
                            ComboBox->AddOption(DisplayActor->GetObjectName());
                        }
                    }
                },
                [this](const TAttributeData& Data)
                {
                    const auto* Display = DP::GetPlaceableActorByGuid<ADP_Display>(GetWorld(), Data.Get<FGuid>());
                    return Display ? Display->GetObjectName() : "";
                });
            break;
        default:
            checkNoEntry();
            break;
    }
    check(AttributeWidget);
    AttributeWidget->Init(AttributeType);
    AttributeWidget->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);

    return AttributeWidget;
}

TObjectPtr<UDP_BaseAttributeWidget> UDP_AttributesListWidget::CreateTextInputAttributeWidget(const FText& DefaultText)
{
    auto* TextInputAttributeWidget = CreateWidget<UDP_TextInputAttributeWidget>(GetWorld(), TextInputAttributeWidgetClass);
    check(TextInputAttributeWidget);
    TextInputAttributeWidget->SetDefaultText(DefaultText);

    return TextInputAttributeWidget;
}

TObjectPtr<UDP_BaseAttributeWidget> UDP_AttributesListWidget::CreateComboBoxAttributeWidget(TComboBoxDataUpdater&& UpdateFunc,
                                                                                            TComboBoxSelectedOptionGetter&& SelectedOptionGetter)
{
    auto* ComboBoxAttributeWidget = CreateWidget<UDP_ComboBoxAttributeWidget>(GetWorld(), ComboBoxAttributeWidgetClass);
    check(ComboBoxAttributeWidget);
    ComboBoxAttributeWidget->SetUpdateFunc(MoveTemp(UpdateFunc));
    ComboBoxAttributeWidget->SetSelectedOptionGetter(MoveTemp(SelectedOptionGetter));

    return ComboBoxAttributeWidget;
}

void UDP_AttributesListWidget::OnAttributeChangedHandler(EAttributeType AttributeType, TAttributeData Data)
{
    OnAttributeChanged.Broadcast(AttributeType, Data);
}
