// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SimpleEditorModule.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "LevelEditor.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "SimpleEditorAnimCustomization.h"

#define LOCTEXT_NAMESPACE "SimpleEditorModule"

IMPLEMENT_MODULE(FSimpleEditorModule, SimpleEditor);

class FSimpleCommands : public TCommands<FSimpleCommands>
{
public:
	FSimpleCommands() : TCommands<FSimpleCommands>(TEXT("SimpleCommands"), LOCTEXT("SimpleCommands", "SSCommands"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override
	{
		CreateCommand("AnimSequence", FExecuteAction::CreateRaw(this, &FSimpleCommands::OnClickTest));
	}

	TMap<TSharedPtr<FUICommandInfo>, FExecuteAction> Commands;
	TMap<TSharedPtr<FUICommandInfo>, FExecuteAction> CheckTableDataCommands;
public:
	
private:
	void MakeUICommand(FBindingContext* This, TSharedPtr< FUICommandInfo >& OutCommand, const TCHAR* OutSubNamespace, const TCHAR* OutCommandName, const TCHAR* OutCommandNameUnderscoreTooltip, const ANSICHAR* DotOutCommandName, const TCHAR* FriendlyName, const TCHAR* InDescription, const EUserInterfaceActionType CommandType, const FInputChord& InDefaultChord)
	{
		static const FString UICommandsStr(TEXT("UICommands"));
		const FString Namespace = OutSubNamespace && FCString::Strlen(OutSubNamespace) > 0 ? UICommandsStr + TEXT(".") + OutSubNamespace : UICommandsStr;

		FUICommandInfo::MakeCommandInfo(
			This->AsShared(),
			OutCommand,
			OutCommandName,
			FInternationalization::ForUseOnlyByLocMacroAndGraphNodeTextLiterals_CreateText(FriendlyName, *Namespace, OutCommandName),
			FInternationalization::ForUseOnlyByLocMacroAndGraphNodeTextLiterals_CreateText(InDescription, *Namespace, OutCommandNameUnderscoreTooltip),
			FSlateIcon(This->GetStyleSetName(), ISlateStyle::Join(This->GetContextName(), DotOutCommandName)),
			CommandType,
			InDefaultChord,
			FInputChord()
		);
	}
	
	void CreateCommand(FString Name, FExecuteAction ExecuteAction)
	{
		TSharedPtr<FUICommandInfo> result;
		//UI_COMMAND_Function(this, result, TEXT(""), *Name, *FString::Printf(TEXT("%s_Tooltip"), *Name), TCHAR_TO_ANSI(*FString::Printf(TEXT(".%s"), *Name)), *Name, *Name, EUserInterfaceActionType::Button, FInputGesture());
		TArray<FStringFormatArg> ParamData;
		ParamData.Add(Name);
		MakeUICommand(this, result, TEXT(""), *Name, *FString::Format(TEXT("{0}_Tooltip"), ParamData), TCHAR_TO_ANSI(*FString::Printf(TEXT(".%s"), *Name)), *Name, *Name, EUserInterfaceActionType::Button, FInputGesture());
		Commands.Add(result, ExecuteAction);
	}

	void OnClickTest()
	{
	}
};

void FSimpleEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	RegisterPropertyTypeCustomizations();
	RegisterObjectCustomizations();
	RegisterCommands();
	RegisterCustomMenuBar();

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FSimpleEditorModule::ShutdownModule()
{
	FSimpleCommands::Unregister();
	
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		// Unregister all classes customized by name
		for (auto It = RegisteredClassNames.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomClassLayout(*It);
			}
		}

		// Unregister all structures
		for (auto It = RegisteredPropertyTypes.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomPropertyTypeLayout(*It);
			}
		}

		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

void FSimpleEditorModule::RegisterPropertyTypeCustomizations()
{
	RegisterCustomPropertyTypeLayout("SimpleAnimRowHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SimpleEditorAnimCustomization::MakeInstance));
}

void FSimpleEditorModule::RegisterObjectCustomizations()
{

}

void FSimpleEditorModule::RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
	check(ClassName != NAME_None);

	RegisteredClassNames.Add(ClassName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}

void FSimpleEditorModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

void FSimpleEditorModule::RegisterCommands()
{
	FSimpleCommands::Register();

	for (auto& Elem : FSimpleCommands::Get().Commands)	Commands.Add(Elem.Key, Elem.Value);
	for (auto& Elem : FSimpleCommands::Get().CheckTableDataCommands)	TableDataCheckCommands.Add(Elem.Key, Elem.Value);

	UICommandList = MakeShareable(new FUICommandList);
	for (auto& Elem : Commands)
	{
		UICommandList->MapAction(Elem.Key, Elem.Value, FCanExecuteAction());
	}
	//
	for (auto& Elem : TableDataCheckCommands)
	{
		UICommandList->MapAction(Elem.Key, Elem.Value, FCanExecuteAction());
	}
}

void FSimpleEditorModule::RegisterCustomMenuBar()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	// 메뉴바를 어디에다 Add할지에 대한 값 ~> ExtensionHook
	// Edit : 에디터 최상단 메뉴바
	// WindowLayOut : (?)
	// 여러가지가 있음..
	MenuExtender->AddMenuBarExtension("Edit", EExtensionHook::After, UICommandList, FMenuBarExtensionDelegate::CreateRaw(this, &FSimpleEditorModule::ExtendCustomMenuBar));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FSimpleEditorModule::ExtendCommand(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("Commons", LOCTEXT("Commons", "Commons"));
	for (auto& Elem : Commands)
	{
		MenuBuilder.AddMenuEntry(Elem.Key);
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("Extendes", LOCTEXT("Extendes", "Extendes"));
	for (auto& Elem : TableDataCheckCommands)
	{
		MenuBuilder.AddMenuEntry(Elem.Key);
	}
	MenuBuilder.EndSection();
}

void FSimpleEditorModule::ExtendCustomMenuBar(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		FText::FromString("SimpleMenu01"),
		FText::FromString("SimpleMenu02"),
		FNewMenuDelegate::CreateRaw(this, &FSimpleEditorModule::ExtendCommand)
	);
}

#undef LOCTEXT_NAMESPACE