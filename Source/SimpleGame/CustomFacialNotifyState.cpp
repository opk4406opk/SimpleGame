
#include "CustomFacialNotifyState.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"


void UCustomFacialNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	//
	Animation->RawCurveData.FloatCurves.Empty();
	//
	for (FSimpleAnimRowHandle animRowHandle : CompositeAnimRowHandles)
	{
		const bool bValidRowHandle = IsValid(animRowHandle.FacialAnimComposite) == true && animRowHandle.FacialCurve.GetRichCurve() != nullptr;
		if (bValidRowHandle == true)
		{
			FFloatCurve newFloatCurve;
			for (FRichCurveKey curveKey : animRowHandle.FacialCurve.GetRichCurve()->Keys)
			{
				const float offsetedTime = curveKey.Time + StartTime;
				newFloatCurve.Name = animRowHandle.CurveName;
				newFloatCurve.FloatCurve.UpdateOrAddKey(offsetedTime, curveKey.Value);
			}
			//
			if (newFloatCurve.FloatCurve.GetNumKeys() > 0)
			{
				// 1) setting float curve.
				Animation->RawCurveData.FloatCurves.Add(newFloatCurve);
				// 2) update set of (name, flaotCurves)
				Animation->RawCurveData.AddCurveData(animRowHandle.CurveName);
			}
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
	if (IsValid(MeshComp) == true) MeshComp->ClearMorphTargets();
	if (IsValid(Animation) == true) Animation->RawCurveData.FloatCurves.Empty();
}
