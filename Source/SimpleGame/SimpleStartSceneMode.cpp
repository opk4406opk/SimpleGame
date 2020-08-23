// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleStartSceneMode.h"
#include "SimpleGame/SimpleUserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ASimpleStartSceneMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//
	UUserWidget* instancedWidget = CreateWidget(GetWorld(), UserInterfaceWidgetClass);
	UserInterfaceWidget = Cast<USimpleUserWidget>(instancedWidget);
	if (IsValid(UserInterfaceWidget) == true)
	{
		UserInterfaceWidget->Init();
		UserInterfaceWidget->AddToViewport();
	}
	UE_LOG(LogTemp, Display, TEXT("ASimpleStartSceneMode::InitGame"));
}

void ASimpleStartSceneMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Display, TEXT("ASimpleStartSceneMode::StartPlay"));
	//UMobilePatchingLibrary::RequestContent();
}

void ASimpleStartSceneMode::OnClickStartGame()
{
	UGameplayStatics::OpenLevel(this, TEXT("InGame"));
}
