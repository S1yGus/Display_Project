// Display_Project, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Display_ProjectEditorTarget : TargetRules
{
    public Display_ProjectEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "Display_Project" });
    }
}
