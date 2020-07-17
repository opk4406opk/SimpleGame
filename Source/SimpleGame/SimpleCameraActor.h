// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SimpleCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API ASimpleCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	void OnTouchBegin(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchEnd(ETouchIndex::Type FingerIndex, FVector Location);
	void OnFingerMove(ETouchIndex::Type FingerIndex, FVector Location);
private:
	/** The normalized screen location when a drag starts */
	FVector2D StartTouchLocation;
	FVector2D LastTouchLocation;
	FVector2D EndTouchLocation;
	FVector2D LastDirection;

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
};
