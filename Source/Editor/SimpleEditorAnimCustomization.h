// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PropertyHandle.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

class SimpleEditorAnimCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	// Inherited via IPropertyTypeCustomization
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> PropertyHandle, class FDetailWidgetRow & HeaderRow, IPropertyTypeCustomizationUtils & CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder & ChildBuilder, IPropertyTypeCustomizationUtils & CustomizationUtils) override;
private:
	FReply OnClickAssign();
private:
	TSharedPtr<IPropertyHandle> RootPropertyHandle;
	IDetailChildrenBuilder* RootStructBuilder;
	//
	TSharedPtr<IPropertyHandle> CompositeAnimPropertyHandle;
	TSharedPtr<IPropertyHandle> CurvePropertyHandle;
	TSharedPtr<IPropertyHandle> CurveSmartNamePropertyHandle;
};
