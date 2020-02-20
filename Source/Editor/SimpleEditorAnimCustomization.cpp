#include "SimpleEditorAnimCustomization.h"
#include "Runtime/Slate/Public/Widgets/Text/STextBlock.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Animation/AnimComposite.h"
//
#define LOCTEXT_NAMESPACE "SimpleEditorAnimCustomization"
//

TSharedRef<IPropertyTypeCustomization> SimpleEditorAnimCustomization::MakeInstance()
{
	return MakeShareable(new SimpleEditorAnimCustomization);
}

void SimpleEditorAnimCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> PropertyHandle, class FDetailWidgetRow & HeaderRow, IPropertyTypeCustomizationUtils & CustomizationUtils)
{
	RootPropertyHandle = PropertyHandle;
	CompositeAnimPropertyHandle = PropertyHandle->GetChildHandle(0).ToSharedRef(); // 0 번째는 컴포짓애님.
	CurvePropertyHandle = PropertyHandle->GetChildHandle(1).ToSharedRef(); // 1번째는 커브값.
	CurveSmartNamePropertyHandle = PropertyHandle->GetChildHandle(2).ToSharedRef(); // 2번째는 커브값에 대한 SmartName.
	HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget(
				FText::FromString(TEXT("CompositeAnim(Facial)")),
				FText::GetEmpty(),
				false)
		];
}

void SimpleEditorAnimCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder & ChildBuilder, IPropertyTypeCustomizationUtils & CustomizationUtils)
{
	RootStructBuilder = &ChildBuilder;
	ChildBuilder.AddProperty(CurvePropertyHandle.ToSharedRef());
	ChildBuilder.AddProperty(CompositeAnimPropertyHandle.ToSharedRef());
	ChildBuilder.AddProperty(CurveSmartNamePropertyHandle.ToSharedRef());
	ChildBuilder.AddCustomRow(FText::FromString(TEXT("Assign Curve from CompositeAnim")))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::GetEmpty())
			.Font(CustomizationUtils.GetRegularFont())
		]
		.ValueContent()
		.MaxDesiredWidth(0.0f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(20.0f, 2.0f))
			.Text(LOCTEXT("", "Assign Curve from CompositeAnim"))
			.OnClicked(this, &SimpleEditorAnimCustomization::OnClickAssign)
		];
}

FReply SimpleEditorAnimCustomization::OnClickAssign()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickAssign"));
	//  get compositeAnim property.
	UObject* outCompAnim = nullptr;
	CompositeAnimPropertyHandle->GetValue(outCompAnim);
	//
	UAnimComposite* compositeAnimPtr = Cast<UAnimComposite>(outCompAnim);
	if (IsValid(compositeAnimPtr) == true)
	{
		TArray<FFloatCurve> floatCurves = compositeAnimPtr->GetCurveData().FloatCurves;
		// get curve property.
		TArray<void*> outCurveStructs;
		CurvePropertyHandle->AccessRawData(outCurveStructs);
		FRuntimeFloatCurve* curvePtr = reinterpret_cast<FRuntimeFloatCurve*>(outCurveStructs[0]);
		if (curvePtr != nullptr)
		{
			// reset.
			curvePtr->GetRichCurve()->Reset();
			// Update.
			for (FFloatCurve curve : floatCurves)
			{
				TArray<float> outTimes;
				TArray<float> outValues;
				curve.GetKeys(outTimes, outValues);
				for (int32 idx = 0; idx < outTimes.Num(); ++idx)
				{
					curvePtr->GetRichCurve()->UpdateOrAddKey(outTimes[idx], outValues[idx]);
				}
			}
			// Curve 값에 대한 name 추출.
			if (floatCurves.Num() > 0)
			{
				FSmartName curveName = floatCurves[0].Name;
				// get smart name property.
				TArray<void*> outSmartNameStructs;
				CurveSmartNamePropertyHandle->AccessRawData(outSmartNameStructs);
				// setting.
				FSmartName* smartNamePtr = reinterpret_cast<FSmartName*>(outSmartNameStructs[0]);
				smartNamePtr->DisplayName = curveName.DisplayName;
				smartNamePtr->UID = curveName.UID;
			}
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE