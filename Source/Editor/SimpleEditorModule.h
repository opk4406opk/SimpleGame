// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "PropertyEditorDelegates.h"
#include "CoreUObject.h"

class FSimpleEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override { return true; }

private:
	void RegisterPropertyTypeCustomizations();
	void RegisterObjectCustomizations();
	void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);
	void RegisterCommands();
	void RegisterCustomMenuBar();
	void ExtendCommand(FMenuBuilder& MenuBuilder);
	void ExtendCustomMenuBar(FMenuBarBuilder& MenuBarBuilder);

	TSet<FName> RegisteredClassNames;
	TSet<FName> RegisteredPropertyTypes;

	TSharedPtr<FUICommandList> UICommandList;
	TMap<TSharedPtr<FUICommandInfo>, FExecuteAction> Commands;
	TMap<TSharedPtr<FUICommandInfo>, FExecuteAction> TableDataCheckCommands;
};