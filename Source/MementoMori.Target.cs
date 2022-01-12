

using UnrealBuildTool;
using System.Collections.Generic;

public class MementoMoriTarget : TargetRules
{
	public MementoMoriTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "MementoMori" } );
	}
}
