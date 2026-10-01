using UnrealBuildTool;

public class RarityPoolEditor : ModuleRules
{
    public RarityPoolEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "Core", "AssetTools", "RarityPool", "UMG", "Slate",
                "Blutility",
                "EditorScriptingUtilities",
                "BlueprintGraph",
                "PropertyEditor",
                "ImageWrapper",
                "GraphEditor",
                "EditorStyle",
                "InputCore",
                
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", "RarityPool", "AssetTools", "UMG", "Slate",
                "Blutility",
                "EditorScriptingUtilities",
                "BlueprintGraph",
                "PropertyEditor",
                "ImageWrapper",
                "GraphEditor",
                "EditorStyle",
                "InputCore"
            }
        );
    }
}