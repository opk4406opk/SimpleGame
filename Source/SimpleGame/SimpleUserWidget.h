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
	bool IsMultiTouch();
	
protected:
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

};
