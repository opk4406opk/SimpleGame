#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"

UENUM(BlueprintType)
enum class EGameAntialiasingMethod : uint8
{
	/*
	Engine default (project setting) for AntiAliasingMethod is (postprocess volume/camera/game setting still can override)
	0: off (no anti-aliasing)
	1: FXAA (faster than TemporalAA but much more shimmering for non static cases)
	2: TemporalAA (default)
	3: MSAA (Forward shading only)
	*/
	Off,
	FXAA,
	TemporalAA,
	MSAA
};
UENUM(BlueprintType)
enum class EGameMSAASamplingValue : uint8
{
	/*
	Number of MSAA samples to use with the forward renderer. Only used when MSAA is enabled in the rendering project settings.
	0: MSAA disabled (Temporal AA enabled)
	1: MSAA disabled
	2: Use 2x MSAA
	4: Use 4x MSAA
	8: Use 8x MSAA
	*/
	MSAA_Disabled_And_TemrpoalAA_Enabled = 0,
	MSAA_Disabled = 1,
	MSAA_2x = 2,
	MSAA_4x = 4,
	MSAA_8x = 8
};

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	None,
	DynamicShadow,
	Water,
};