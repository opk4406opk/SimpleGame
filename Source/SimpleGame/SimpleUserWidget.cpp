// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleUserWidget.h"

void USimpleUserWidget::SetLogText(FString logText)
{
	if (IsValid(LogTextBlock) == true)
	{
		LogTextBlock->SetText(FText::FromString(logText));
	}
}