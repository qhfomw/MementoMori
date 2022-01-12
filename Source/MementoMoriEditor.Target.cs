

using UnrealBuildTool;
using System.Collections.Generic;

public class MementoMoriEditorTarget : TargetRules
{
	public MementoMoriEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "MementoMori" } );
	}
}
