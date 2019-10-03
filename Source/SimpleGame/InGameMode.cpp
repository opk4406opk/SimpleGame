// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UUserWidget* instancedWidget = CreateWidget(GetWorld(), UserInterfaceWidgetClass);
	UserInterfaceWidget = Cast<USimpleUserWidget>(instancedWidget);
	if (UserInterfaceWidget != nullptr)
	{
		UserInterfaceWidget->AddToViewport();
	}
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

void AInGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

void AInGameMode::LoadOtherLevel()
{
	LoadLevelInstance(SimpleGameDataAsset->OhterLevel);
}

void AInGameMode::UnLoadOtherLevel()
{
	UnLoadLevelInstance(SimpleGameDataAsset->OhterLevel);
}

void AInGameMode::LoadSimpleLevelWithLS(bool IncludeLS)
{
	LoadLevelInstance(SimpleGameDataAsset->SimpleLevel);
	if (IncludeLS == true)
	{
		LoadLevelInstance(SimpleGameDataAsset->LS_SimpleLevel);
	}
}

void AInGameMode::UnLoadSimpleLevelWithLS(bool IncludeLS)
{
	UnLoadLevelInstance(SimpleGameDataAsset->SimpleLevel);
	if (IncludeLS == true)
	{
		UnLoadLevelInstance(SimpleGameDataAsset->LS_SimpleLevel);
	}
}

void AInGameMode::LoadSubStreamOtherLevel()
{
	LoadSubLevelStream();
}

void AInGameMode::UnLoadSubStreamOtherLevel()
{
	UnLoadSubLevelStream();
}

void AInGameMode::LoadLargeLevel()
{
	LoadLevelInstance(SimpleGameDataAsset->BigLargeLevel);
}

void AInGameMode::UnLoadLargeLevel()
{
	UnLoadLevelInstance(SimpleGameDataAsset->BigLargeLevel);
}

void AInGameMode::LoadMidiumLevel()
{
	LoadLevelInstance(SimpleGameDataAsset->MidiumLevel);
}

void AInGameMode::UnLoadMidiumLevel()
{
	UnLoadLevelInstance(SimpleGameDataAsset->MidiumLevel);
}

void AInGameMode::LoadLevelInstance(TSoftObjectPtr<UWorld> Level)
{
	if (GEngine)
	{
		GEngine->ClearOnScreenDebugMessages();
		GEngine->AddOnScreenDebugMessage(-1, 99999.0f, FColor::Green, TEXT("Start Stream Sub level..."));
	}
	UserInterfaceWidget->SetLogText(FString("Start Stream Sub level..."));
	//
	bool result = false;
	CurrentLevelStreaming = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), Level, FVector::ZeroVector, FRotator::ZeroRotator, result);
	if (result == false)
	{
		UE_LOG(LogTemp, Error, TEXT("trying to load invalid level %s"), *Level.GetLongPackageName());
		return;
	}

	OnFinishLoadSubLevelDelegate.Clear();
	OnFinishLoadSubLevelDelegate.BindUFunction(this, "OnFinishLoadSubLevel");
	CurrentLevelStreaming->OnLevelShown.Add(OnFinishLoadSubLevelDelegate);

	GEngine->ClearOnScreenDebugMessages();
	auto arr = GetWorld()->GetStreamingLevels();
	for (ULevelStreaming* levelStream : GetWorld()->GetStreamingLevels())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sub level name : %s"), levelStream->PackageNameToLoad);
		UE_LOG(LogTemp, Error, TEXT("Sub level name %s"), *levelStream->PackageNameToLoad.ToString());
	}
}

void AInGameMode::UnLoadLevelInstance(TSoftObjectPtr<UWorld> Level)
{
	UWorld* const world = GetWorld();
	if (!world) return;

	int32 index = world->GetStreamingLevels().IndexOfByPredicate([&](const ULevelStreaming* param) {
		return param->PackageNameToLoad == FName(*Level.GetLongPackageName()); });
	if (index == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("trying to unload invalid level %s"), *Level.GetLongPackageName());
		return;
	}

	ULevelStreaming* streamingLevel = world->GetStreamingLevels()[index];
	if (ULevel* level = streamingLevel->GetLoadedLevel())
	{
		streamingLevel->GetWorld()->RemoveFromWorld(level);
	}
	world->RemoveStreamingLevel(streamingLevel);
}

void AInGameMode::LoadSubLevelStream()
{
	FLatentActionInfo LatentInfo;
	auto ttt = SimpleGameDataAsset->OhterLevel.GetAssetName();
	UGameplayStatics::LoadStreamLevel(GetWorld(), *SimpleGameDataAsset->OhterLevel.GetAssetName(), true, false, LatentInfo);
	ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(GetWorld(), *SimpleGameDataAsset->OhterLevel.GetAssetName());
	if (level == nullptr)
	{
		if (GEngine)
		{
			GEngine->ClearOnScreenDebugMessages();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UGameplayStatics::LoadStreamLevel is Failed..."));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->ClearOnScreenDebugMessages();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("UGameplayStatics::LoadStreamLevel is Success~"));
		}
	}
}

void AInGameMode::UnLoadSubLevelStream()
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), *SimpleGameDataAsset->OhterLevel.GetAssetName(), LatentInfo, false);
}

void AInGameMode::OnFinishLoadSubLevel()
{
	if (IsValid(UserInterfaceWidget) == true)
	{
		if (GEngine)
		{
			GEngine->ClearOnScreenDebugMessages();
			GEngine->AddOnScreenDebugMessage(-1, 99999.0f, FColor::Green, TEXT("Finish Stream Sub level..."));
		}
		UserInterfaceWidget->SetLogText(FString("Finish Stream Sub level..."));
	}
	GetWorld()->FlushLevelStreaming();
}
