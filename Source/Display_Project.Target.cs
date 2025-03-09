// Display_Project, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Display_ProjectTarget : TargetRules
{
    public Display_ProjectTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "Display_Project" });
    }
}
