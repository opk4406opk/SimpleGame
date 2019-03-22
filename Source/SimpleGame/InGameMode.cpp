// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("AInGameMode::InitGame"));
}

void AInGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("AInGameMode::StartPlay"));
	//
	GamePlayerCharacter.LoadSynchronous();
	GamePlayerInstance = GetWorld()->SpawnActor<AGamePlayerCharacter>(GamePlayerCharacter->GetClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
#if UE_EDITOR
	GamePlayerInstance->SetActorLabel(*FString("GamePlayerCharacter_Instance"));
#endif
}

void AInGameMode::TestAAMethod(EGameAntialiasingMethod methodType)
{
	//test - AA 퀼리티값 4(시네마틱)으로 설정 ( 테스트용도로..)
	IConsoleManager::Get().FindConsoleVariable(TEXT("sg.AntiAliasingQuality"))->Set(4.0f);
	//
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"))->Set(0.0f);
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.mobilemsaa"))->Set(1.0f);
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.msaacount"))->Set(1.0f);
	switch (methodType)
	{
	case EGameAntialiasingMethod::Off:
		IConsoleManager::Get().FindConsoleVariable(TEXT("sg.AntiAliasingQuality"))->Set(0.0f);
		break;
	case EGameAntialiasingMethod::FXAA:
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"))->Set(2.0f);
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
		EGameMSAASamplingValue hardwareSampleValue = GetSSGameInstance()->GetSavedData()->MSAASampling;
		if (hardwareSampleValue == EGameMSAASamplingValue::MSAA_Disabled || hardwareSampleValue == EGameMSAASamplingValue::MSAA_Disabled_And_TemrpoalAA_Enabled)
		{
			// 디바이스 하드웨어에서 MSAA를 지원하지 않는경우 별도의 메세지 출력?
			return;
		}
		//
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
		break;
	}
	IConsoleVariable* antialiasingMethod = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));
	antialiasingMethod->Set((float)methodType);
}
