// Display_Project, all rights reserved.

#include "UI/DP_OptionsWidget.h"
#include "UI/DP_ButtonWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"

void UDP_OptionsWidget::Init(const FVideoQualityOptionsData& VideoQualityOptionsData, float RotationSpeedNormalized, float SoundVolume)
{
    VideoQualityComboBox->ClearOptions();
    for (const auto& Option : VideoQualityOptionsData.VideoQualityOptions)
    {
        VideoQualityComboBox->AddOption(Option);
    }
    VideoQualityComboBox->SetSelectedOption(VideoQualityOptionsData.CurrentVideoQualityOption);
    RotationSpeedSlider->SetValue(RotationSpeedNormalized);
    SoundVolumeSlider->SetValue(SoundVolume);
}

void UDP_OptionsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(VideoQualityComboBox);
    check(RotationSpeedSlider);
    check(SoundVolumeSlider);
    check(BackButton);

    VideoQualityComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnVideoQualitySelectionChangedHandler);
    RotationSpeedSlider->OnMouseCaptureEnd.AddDynamic(this, &ThisClass::OnRotationSpeedChangedHandler);
    SoundVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &ThisClass::OnSoundVolumeChangedHandler);
    BackButton->OnClicked.AddUObject(this, &ThisClass::OnClickedBackButtonHandler);
}

void UDP_OptionsWidget::OnVideoQualitySelectionChangedHandler(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto Quality = static_cast<EVideoQuality>(VideoQualityComboBox->GetSelectedIndex());
    OnVideoQualityChanged.Broadcast(Quality);
}

void UDP_OptionsWidget::OnRotationSpeedChangedHandler()
{
    OnRotationSpeedChanged.Broadcast(RotationSpeedSlider->GetNormalizedValue());
}

void UDP_OptionsWidget::OnSoundVolumeChangedHandler()
{
    OnSoundVolumeChanged.Broadcast(SoundVolumeSlider->GetNormalizedValue());
}

void UDP_OptionsWidget::OnClickedBackButtonHandler()
{
    OnStopShowingOptions.Broadcast();
}
