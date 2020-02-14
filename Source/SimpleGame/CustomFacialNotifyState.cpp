// Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "CustomFacialNotifyState.h"

void UCustomFacialNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	//
	if (IsValid(FacialAnimComposite) == true)
	{
		const FRawCurveTracks& curveData = FacialAnimComposite->GetCurveData();
		for (FFloatCurve floatCurve : curveData.FloatCurves)
		{
			TArray<float> outTimes;
			TArray<float> outValues;
			floatCurve.GetKeys(outTimes, outValues);
			// Offset. (notify state의 시작시작을 offset으로 해서 더해준다.)
			for (float& curveTime : outTimes)
			{
				curveTime += StartTime;
			}
			// Reset.
			floatCurve.FloatCurve.Reset();
			// Update.
			for (int32 idx = 0; idx < outTimes.Num(); ++idx)
			{
				floatCurve.UpdateOrAddKey(outValues[idx], outTimes[idx]);
			}
			//
			Animation->RawCurveData.FloatCurves.Add(floatCurve);
		}
	}
}

void UCustomFacialNotifyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCustomFacialNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	ResetMorphDatas(MeshComp, Animation);
}

void UCustomFacialNotifyState::OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& ContainingAnimNotifyEvent)
{
	Super::OnAnimNotifyCreatedInEditor(ContainingAnimNotifyEvent);
}

void UCustomFacialNotifyState::ResetMorphDatas(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// clear morphtarget's curve values.
	if(IsValid(MeshComp) == true) MeshComp->ClearMorphTargets();
	if(IsValid(Animation) == true) Animation->RawCurveData.FloatCurves.Empty();
}
