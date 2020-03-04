// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Animation/AnimComposite.h"
#include "SimpleStructs.h"
#include "SimpleFacialDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API USimpleFacialDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FSimpleAnimRowHandle> CompositeAnimRowHandles;
};
