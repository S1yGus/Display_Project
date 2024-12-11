// Display_Project, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DP_SettingsSave.generated.h"

UCLASS()
class DISPLAY_PROJECT_API UDP_SettingsSave : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float RotationSpeedNormalized{0.5f};

    UPROPERTY(Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SoundVolume{0.5f};
};
