// Display_Project, all rights reserved.
// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DP_BaseAnimatedWidget.h"
#include "DP_CoreTypes.h"
#include "DP_OptionsWidget.generated.h"

class UDP_ButtonWidget;
class UComboBoxString;
class USlider;

UCLASS()
class DISPLAY_PROJECT_API UDP_OptionsWidget : public UDP_BaseAnimatedWidget
{
    GENERATED_BODY()

public:
    FOnBackSignature OnBack;
    FOnVideoQualityChangedSignature OnVideoQualityChanged;
    FOnRotationSpeedChangedSignature OnRotationSpeedChanged;
    FOnSoundVolumeChangedSignature OnSoundVolumeChanged;

    void Init(const FVideoQualityOptionsData& VideoQualityOptionsData, float RotationSpeedNormalized, float SoundVolume);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UComboBoxString> VideoQualityComboBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<USlider> RotationSpeedSlider;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<USlider> SoundVolumeSlider;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UDP_ButtonWidget> BackButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnVideoQualitySelectionChangedHandler(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION()
    void OnRotationSpeedChangedHandler();
    UFUNCTION()
    void OnSoundVolumeChangedHandler();
    void OnClickedBackButtonHandler();
};
