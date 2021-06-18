// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "SimpleActor.generated.h"

UCLASS()
class SIMPLEGAME_API ASimpleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleActor();
public:
	UPROPERTY(EditAnywhere)
	FRotator Rotator;
	UPROPERTY(EditAnywhere)
	bool bLocalSpace;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual UFUNCTION() void OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual UAnimMontage* PlayAnimationSequence(UAnimSequenceBase* Asset, float BlendInTime = 0.25f, float BlendOutTime = 0.25f, float InPlayRate = 1.f, int32 LoopCount = 1, float InTimeToStartMontageAt = 0.f);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void RotateActor();
private:
	
	UPROPERTY(Transient) USkeletalMeshComponent* MeshComponent = nullptr;
	UPROPERTY(Transient) UCapsuleComponent* CapsuleComponent = nullptr;
	bool bTicking = false;
};
