// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DP_CoreDelegates.h"
#include "DP_ButtonWidget.generated.h"

class UImage;
class UButton;
class UTexture2D;

UCLASS()
class DISPLAY_PROJECT_API UDP_ButtonWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedSignature OnClicked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    TObjectPtr<UTexture2D> Thumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FLinearColor HoveredTint{0.55f, 0.55f, 0.55f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FLinearColor UnhoveredTint{1.0f, 1.0f, 1.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FLinearColor PressedTint{0.35f, 0.35f, 0.35f, 1.0f};

    void Init(UTexture2D* InThumbnail);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> Button;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UImage> ButtonBackgroundImage;

    UPROPERTY(Meta = (BindWidgetOptional))
    TObjectPtr<UImage> ButtonImage;

    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;

private:
    UFUNCTION()
    void OnClickedHandler();
    UFUNCTION()
    void OnHoveredHandler();
    UFUNCTION()
    void OnUnhoveredHandler();
    UFUNCTION()
    void OnPressedHandler();
};
