// Display_Project, all rights reserved.

#include "Framework/DP_GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Save/DP_SettingsSave.h"
#include "AudioDevice.h"
#include "AudioThread.h"
#include "DP_Utils.h"

namespace
{

const FString SettingsSaveSlotName{"SettingsSave"};
const FString MasterSoundClassName{"Master"};
const float MinRotationSpeed{0.1f};
const float MaxRotationSpeed{2.0f};

void FillVideoQualityOptions(FVideoQualityOptionsData& VideoQualityOptionsData)
{
    const UEnum* VideoQualityEnum = StaticEnum<EVideoQuality>();
    for (int32 i = 0; i < VideoQualityEnum->NumEnums() - 1; ++i)
    {
        const auto CurrentQuality = static_cast<EVideoQuality>(i);
        VideoQualityOptionsData.VideoQualityOptions.Add(DP::EnumToString(CurrentQuality));
    }
}

}    // namespace

UDP_GameUserSettings* UDP_GameUserSettings::Get()
{
    return GEngine ? Cast<UDP_GameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void UDP_GameUserSettings::ToggleScreenMode()
{
    const EWindowMode::Type CurrentMode = GetFullscreenMode();
    if (TArray<FIntPoint> Resolutions; CurrentMode != EWindowMode::Windowed)
    {
        UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions);
        if (!Resolutions.IsEmpty())
        {
            SetScreenResolution(Resolutions[0]);
        }
    }
    SetFullscreenMode(CurrentMode == EWindowMode::Type::Windowed ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Windowed);

    ApplyResolutionSettings(false);
}

void UDP_GameUserSettings::SetVideoQuality(EVideoQuality Quality)
{
    const auto Value = static_cast<int32>(Quality);
    SetViewDistanceQuality(Value);
    SetAntiAliasingQuality(Value);
    SetPostProcessingQuality(Value);
    SetShadowQuality(Value);
    SetGlobalIlluminationQuality(Value);
    SetReflectionQuality(Value);
    SetTextureQuality(Value);
    SetVisualEffectQuality(Value);
    SetFoliageQuality(Value);
    SetShadingQuality(Value);

    ApplyNonResolutionSettings();
}

FVideoQualityOptionsData UDP_GameUserSettings::GetVideoQualityOptionsData() const
{
    FVideoQualityOptionsData VideoQualityOptionsData;
    FillVideoQualityOptions(VideoQualityOptionsData);
    VideoQualityOptionsData.CurrentVideoQualityOption = DP::EnumToString(static_cast<EVideoQuality>(GetShadowQuality()));

    return VideoQualityOptionsData;
}

void UDP_GameUserSettings::SetRotationSpeed(float RotationSpeedNormalized)
{
    SettingsSave->RotationSpeedNormalized = RotationSpeedNormalized;
    SaveSettings();
}

float UDP_GameUserSettings::GetRotationSpeedNormalized() const
{
    return SettingsSave->RotationSpeedNormalized;
}

float UDP_GameUserSettings::GetRotationSpeed() const
{
    return FMath::GetMappedRangeValueClamped(TRange<float>(0.0f, 1.0f), TRange<float>(MinRotationSpeed, MaxRotationSpeed), GetRotationSpeedNormalized());
}

void UDP_GameUserSettings::SetSoundVolume(float SoundVolume)
{
    SetSoundClassVolume(MasterSoundClassName, SoundVolume);
    SettingsSave->SoundVolume = SoundVolume;
    SaveSettings();
}

float UDP_GameUserSettings::GetSoundVolume() const
{
    return SettingsSave->SoundVolume;
}

void UDP_GameUserSettings::LoadSettings(bool bForceReload)
{
    Super::LoadSettings(bForceReload);

    CheckSettingsSave();
}

void UDP_GameUserSettings::SaveSettings()
{
    Super::SaveSettings();

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void UDP_GameUserSettings::CheckSettingsSave()
{
    if (SettingsSave)
        return;

    if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlotName, 0))
    {
        SettingsSave = Cast<UDP_SettingsSave>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
    }
    else
    {
        SettingsSave = Cast<UDP_SettingsSave>(UGameplayStatics::CreateSaveGameObject(UDP_SettingsSave::StaticClass()));
        RunHardwareBenchmark();
        ApplySettings(false);
    }

    check(SettingsSave);
}

void UDP_GameUserSettings::SetSoundClassVolume(const FString& SoundClassName, float NewVolume)
{
    FAudioThread::RunCommandOnAudioThread(
        [=]()
        {
            if (!GEngine)
                return;

            FAudioDeviceHandle ADevice = GEngine->GetMainAudioDevice();
            if (!ADevice)
                return;

            for (const auto& [SoundClass, SoundClassProperties] : ADevice->GetSoundClassPropertyMap())
            {
                if (SoundClass && SoundClass->GetFullName().Find(SoundClassName) != INDEX_NONE)
                {
                    SoundClass->Properties.Volume = NewVolume;
                }
            }
        });
}
