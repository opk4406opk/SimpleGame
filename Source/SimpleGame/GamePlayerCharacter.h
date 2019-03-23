// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayerCharacter.generated.h"

UCLASS()
class SIMPLEGAME_API AGamePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGamePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveTouchPadX(float x);
	void MoveTouchPadY(float y);
	//
	void RotationTouchPadX(float x);
	void RotationOtuchPadY(float y);

	FRotator LatestTouchRotation;
};
