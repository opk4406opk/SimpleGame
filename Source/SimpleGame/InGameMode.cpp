// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Display, TEXT("AInGameMode::InitGame"));
}

void AInGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Display, TEXT("AInGameMode::StartPlay"));
	/*UClass* playerClass = GamePlayerCharacter.LoadSynchronous();
	if (playerClass != nullptr)
	{
		GamePlayerInstance = GetWorld()->SpawnActor<AGamePlayerCharacter>(playerClass, FVector(0, 0, 0), FRotator(0, 0, 0));
#if UE_EDITOR
		GamePlayerInstance->SetActorLabel(*FString("GamePlayerCharacter_Instance"));
#endif
	}*/
}

void AInGameMode::TestAAMethod(EGameAntialiasingMethod methodType)
{
	if (GEngine)
	{
		GEngine->ClearOnScreenDebugMessages();
		GEngine->AddOnScreenDebugMessage(-1, 9999.0f, FColor::Red, FString("AInGameMode::TestAAMethod"));
	}
	//test - AA 퀼리티값 4(시네마틱)으로 설정 ( 테스트용도로..)
	IConsoleManager::Get().FindConsoleVariable(TEXT("sg.AntiAliasingQuality"))->Set(4.0f);
	switch (methodType)
	{
	case EGameAntialiasingMethod::Off:
		IConsoleManager::Get().FindConsoleVariable(TEXT("sg.AntiAliasingQuality"))->Set(0.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"))->Set(0.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.mobilemsaa"))->Set(1.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.msaacount"))->Set(1.0f);
		break;
	case EGameAntialiasingMethod::FXAA:
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"))->Set(6.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.mobilemsaa"))->Set(1.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.msaacount"))->Set(1.0f);
		break;
	case EGameAntialiasingMethod::TemporalAA:
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.mobilemsaa"))->Set(1.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.msaacount"))->Set(0.0f);
		//
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.TemporalAACurrentFrameWeight"))->Set(0.1f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.TemporalAASamples"))->Set(4.0f);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Tonemapper.Sharpen"))->Set(1.0f);
		//
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"))->Set(4.0f);
		break;
	case EGameAntialiasingMethod::MSAA:
		EGameMSAASamplingValue hardwareSampleValue = EGameMSAASamplingValue::MSAA_4x;
		float setValue = 0.0f;
		switch (hardwareSampleValue)
		{
		case EGameMSAASamplingValue::MSAA_2x:
			setValue = 2.0f;
			break;
		case EGameMSAASamplingValue::MSAA_4x:
			setValue = 4.0f;
			break;
		case EGameMSAASamplingValue::MSAA_8x:
			setValue = 8.0f;
			break;
		default:
			break;
		}
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.msaacount"))->Set(setValue);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.mobilemsaa"))->Set(setValue);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"))->Set(5.0f);
		break;
	}
	IConsoleVariable* antialiasingMethod = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));
	antialiasingMethod->Set((float)methodType);
}
