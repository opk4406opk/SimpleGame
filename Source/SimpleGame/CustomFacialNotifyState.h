// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Animation/AnimComposite.h"
#include "SimpleStructs.h"
#include "CustomFacialNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API UCustomFacialNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FSimpleAnimRowHandle> CompositeAnimRowHandles;
	UPROPERTY(EditAnywhere)
	float StartTime = 0.0f;
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	virtual void OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& ContainingAnimNotifyEvent) override;
	void ResetMorphDatas(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation);
};
