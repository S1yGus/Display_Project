// Display_Project, all rights reserved.

#include "UI/DP_ObjectInfoWidget.h"
#include "UI/DP_ObjectHeaderWidget.h"
#include "UI/DP_AttributesListWidget.h"

void UDP_ObjectInfoWidget::Init(EObjectType ObjectType, UTexture2D* Thumbnail, const TSet<EAttributeType>& Attributes)
{
    ObjectHeader->Init(ObjectType, Thumbnail);
    AttributesList->Init(ObjectType, Attributes);
    AttributesList->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChangedHandler);
}

void UDP_ObjectInfoWidget::UpdateAttributes(const FAttributesMap& Attributes)
{
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
    OnAttributeChanged.Broadcast(AttributeType, AttributeData);
}
