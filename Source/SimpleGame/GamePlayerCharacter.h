// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
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
public:
	UPROPERTY(EditAnywhere)
	AActor* TargetActor;
	UPROPERTY(EditAnywhere)
	FRotator TargetActorRotator;
	UPROPERTY(EditAnywhere)
	FRotator PlayerOrbitRotator;
	UPROPERTY(EditAnywhere)
	float OrbitRotateSpeed = 100.0f;
private:
	void MoveTouchPadX(float x);
	void MoveTouchPadY(float y);
	//
	void RotationTouchPadX(float x);
	void RotationOtuchPadY(float y);

	//
	void OrbitRotation();

private:
	FRotator LatestTouchRotation;
	float RotateSpeed = 3.0f;
	float MoveSpeed = 3.0f;
	UPROPERTY(Transient)
	UCameraComponent* CameraComponent;
	UPROPERTY(Transient)
	USkeletalMeshComponent* MeshComponent;
};
