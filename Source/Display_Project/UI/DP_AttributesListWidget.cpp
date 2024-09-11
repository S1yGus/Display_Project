// Display_Project, all rights reserved.

#include "UI/DP_AttributesListWidget.h"
#include "UI/DP_BaseAttributeWidget.h"
#include "UI/DP_TextInputAttributeWidget.h"
#include "UI/DP_ComboBoxAttributeWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "World/DP_Display_1.h"
#include "DP_Utils.h"

void UDP_AttributesListWidget::Init(EObjectType ObjectType, const FAttributesMap& Attributes)
{
    Type = ObjectType;

    AttributesBox->ClearChildren();
    for (const auto& [AttributeType, Attribute] : Attributes)
    {
        AttributesBox->AddChild(CreateAttributeWidget(AttributeType, Attribute));
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

void UDP_AttributesListWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(AttributesBox);
}

TObjectPtr<UDP_BaseAttributeWidget> UDP_AttributesListWidget::CreateAttributeWidget(EAttributeType AttributeType, const FAttribute& Attribute)
{
    TObjectPtr<UDP_BaseAttributeWidget> AttributeWidget;
    switch (AttributeType)
    {
        case EAttributeType::ObjectName:
            AttributeWidget = CreateTextInputAttributeWidget(UI::ObjectTypeToText(Type));
            break;
        case EAttributeType::DisplayText:
            [[fallthrough]];
        case EAttributeType::ButtonText:
            AttributeWidget = CreateTextInputAttributeWidget(FText::GetEmpty());
            break;
        case EAttributeType::Display:
            AttributeWidget = CreateComboBoxAttributeWidget(
                [this](TObjectPtr<UComboBoxString> ComboBox, TArray<FAttributeData>& AttributeData)
                {
                    TArray<AActor*> DisplayActors;
                    UGameplayStatics::GetAllActorsOfClass(this, ADP_Display_1::StaticClass(), DisplayActors);
                    for (auto* Actor : DisplayActors)
                    {
                        auto* DisplayActor = Cast<ADP_Display_1>(Actor);
                        FAttributeData Data;
                        Data.Set<TObjectPtr<ADP_Display_1>>(DisplayActor);
                        AttributeData.Emplace(Data);
                        ComboBox->AddOption(DisplayActor->GetObjectName());
                    }
                });
            break;
        default:
            checkNoEntry();
            break;
    }
    check(AttributeWidget);
    AttributeWidget->Init(AttributeType, Attribute);
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

TObjectPtr<UDP_BaseAttributeWidget> UDP_AttributesListWidget::CreateComboBoxAttributeWidget(FComboBoxDataUpdater&& UpdateFunc)
{
    auto* ComboBoxAttributeWidget = CreateWidget<UDP_ComboBoxAttributeWidget>(GetWorld(), ComboBoxAttributeWidgetClass);
    check(ComboBoxAttributeWidget);
    ComboBoxAttributeWidget->SetUpdateFunc(MoveTemp(UpdateFunc));

    return ComboBoxAttributeWidget;
}

void UDP_AttributesListWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData Data)
{
    OnAttributeChanged.Broadcast(AttributeType, Data);
}
