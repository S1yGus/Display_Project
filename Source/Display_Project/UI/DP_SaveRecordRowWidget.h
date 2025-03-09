// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_TextButtonWidget.h"
#include "DP_CoreDelegates.h"
#include "DP_SaveRecordRowWidget.generated.h"

class UImage;

UCLASS()
class DISPLAY_PROJECT_API UDP_SaveRecordRowWidget : public UDP_TextButtonWidget
{
    GENERATED_BODY()

public:
    FOnSelectedSaveRecordSignature OnSelectedSaveRecord;

    void Init(const FText& SaveName, const FGuid& SaveGuid);
    bool SetSelectd(const FGuid& SelectdGuid);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UImage> Frame;

    virtual void NativeOnInitialized() override;

private:
    FGuid Guid;

    void OnClickedHandler();
};
