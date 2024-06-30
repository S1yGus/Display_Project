// Display_Project, all rights reserved.

using UnrealBuildTool;

public class Display_Project : ModuleRules
{
    public Display_Project(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PublicIncludePaths.AddRange(new string[] { "Display_Project/Interfaces", "Display_Project/World" });
    }
}
