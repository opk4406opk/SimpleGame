// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "SimpleUserWidget.generated.h"

class SEditableTextBox;
/**
 * 
 */
UCLASS()
class SIMPLEGAME_API USimpleUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetLogText(FString logText);
	void Init();
public:
	UPROPERTY(EditDefaultsOnly)
	UTextBlock* LogTextBlock;

	UPROPERTY(EditDefaultsOnly) UWidget* MaxExtentWidget = nullptr;
	UPROPERTY(EditDefaultsOnly) UWidget* MinExtentWidget = nullptr;
	UPROPERTY(EditDefaultsOnly) UWidget* PivotWidget = nullptr;
private:
	
};
