// Display_Project, all rights reserved.

#include "UI/DP_ObjectInfoWidget.h"
#include "UI/DP_ObjectHeaderWidget.h"
#include "UI/DP_AttributesListWidget.h"

void UDP_ObjectInfoWidget::Init(EObjectType ObjectType, const TArray<EAttributeType>& Attributes)
{
    ObjectHeader->Init(ObjectType);
    AttributesList->Init(ObjectType, Attributes);
    AttributesList->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
}

void UDP_ObjectInfoWidget::Select(const FString& ObjectName, const FAttributesMap& Attributes)
{
    ObjectHeader->UpdateObjectName(ObjectName);
    AttributesList->Update(Attributes);
}

void UDP_ObjectInfoWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ObjectHeader);
    check(AttributesList);
}

void UDP_ObjectInfoWidget::OnAttributeChangedHandler(EAttributeType AttributeType, FAttributeData AttributeData)
{
    if (AttributeType == EAttributeType::ObjectName)
    {
        ObjectHeader->UpdateObjectName(AttributeData.Get<FString>());
    }

    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}
