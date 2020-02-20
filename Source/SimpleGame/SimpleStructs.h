#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
//
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Animation/AnimComposite.h"

#include "SimpleStructs.generated.h"

USTRUCT()
struct FSimpleAnimRowHandle
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UAnimComposite* FacialAnimComposite;
	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve FacialCurve;
	UPROPERTY(EditAnywhere)
	FSmartName CurveName;
};