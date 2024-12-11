// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DP_CoreTypes.h"
#include "DP_GameUserSettings.generated.h"

class UDP_SettingsSave;

UCLASS()
class DISPLAY_PROJECT_API UDP_GameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    static UDP_GameUserSettings* Get();

    void ToggleScreenMode();

    void SetVideoQuality(EVideoQuality Quality);
    [[nodiscard]] FVideoQualityOptionsData GetVideoQualityOptionsData() const;

    void SetRotationSpeed(float RotationSpeedNormalized);
    [[nodiscard]] float GetRotationSpeedNormalized() const;
    [[nodiscard]] float GetRotationSpeed() const;

    void SetSoundVolume(float SoundVolume);
    [[nodiscard]] float GetSoundVolume() const;

    virtual void LoadSettings(bool bForceReload = false) override;
    virtual void SaveSettings() override;

private:
    UPROPERTY()
    TObjectPtr<UDP_SettingsSave> SettingsSave;

    void CheckSettingsSave();
    void SetSoundClassVolume(const FString& SoundClassName, float NewVolume);
};
