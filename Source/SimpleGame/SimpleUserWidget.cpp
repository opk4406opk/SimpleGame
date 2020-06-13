// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleUserWidget.h"

void USimpleUserWidget::SetLogText(FString logText)
{
	if (IsValid(LogTextBlock) == true)
	{
		LogTextBlock->SetText(FText::FromString(logText));
	}
}

void USimpleUserWidget::Init()
{
	
}

bool USimpleUserWidget::IsMultiTouch()
{
	TArray<bool> touches;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	for (int32 idx = 0; idx <= (int32)ETouchIndex::Type::MAX_TOUCHES; idx++)
	{
		float locX, locY;
		bool bPressed = false;
		playerController->GetInputTouchState((ETouchIndex::Type)idx, locX, locY, bPressed);
		if (bPressed == true) touches.Add(true);
	}
	return touches.Num() >= 2;
}

FReply USimpleUserWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Log, TEXT("USimpleUserWidget::NativeOnMouseWheel [ wheel value : %f ]"), InMouseEvent.GetWheelDelta());
	return FReply::Handled();
}

FReply USimpleUserWidget::NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	EGestureEvent gestureType = InGestureEvent.GetGestureType();
	FString typeToString;
	switch (gestureType)
	{
	case EGestureEvent::Scroll:
		typeToString = FString("Scroll");
		break;
	case EGestureEvent::Magnify:
		typeToString = FString("Magnify");
		break;
	case EGestureEvent::Swipe:
		typeToString = FString("Swipe");
		break;
	case EGestureEvent::Rotate:
		typeToString = FString("Rotate");
		break;
	case EGestureEvent::LongPress:
		typeToString = FString("LongPress");
		break;
	}
	UE_LOG(LogTemp, Log, TEXT("USimpleUserWidget::NativeOnTouchGesture [ Gesture : %s ]"), *typeToString);
	return FReply::Handled();
}

FReply USimpleUserWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogTemp, Log, TEXT("USimpleUserWidget::NativeOnTouchStarted "));
	if (IsMultiTouch() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("USimpleUserWidget::NativeOnTouchStarted [ Multi Touch : false ] "));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("USimpleUserWidget::NativeOnTouchStarted [ Multi Touch : true ] "));
	}

	return FReply::Handled();
}

FReply USimpleUserWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply USimpleUserWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogTemp, Log, TEXT("USimpleUserWidget::NativeOnTouchEnded "));
	return FReply::Handled();
}
