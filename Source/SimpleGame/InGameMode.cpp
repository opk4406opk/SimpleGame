// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/SkyLight.h"
#include "Runtime/Engine/Classes/Engine/Light.h"
#include "Runtime/Engine/Classes/Engine/PostProcessVolume.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include "Runtime/Engine/Classes/Components/SkyLightComponent.h"
#include "Runtime/Engine/Classes/Components/ReflectionCaptureComponent.h"

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
	//
	GamePlayerInstance = GetWorld()->SpawnActor<AGamePlayerCharacter>(SimpleGameDataAsset->GamePlayerCharacterClass.Get(), FVector(0, 0, 0), FRotator(0, 0, 0));
#if UE_EDITOR
	GamePlayerInstance->SetActorLabel(*FString("GamePlayerCharacter_Instance"));
#endif
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

void AInGameMode::LoadOtherLevel(ELevelType LevelType)
{
	switch (LevelType)
	{
	case ELevelType::None:
		break;
	case ELevelType::DynamicShadow:
		DisablePersistentLightings();
		LoadLevelInstance(SimpleGameDataAsset->DynamicShadowLevel);
		break;
	case ELevelType::Water:
		DisablePersistentLightings();
		LoadLevelInstance(SimpleGameDataAsset->WaterLevel);
		break;
	default:
		break;
	}
}

void AInGameMode::UnLoadOtherLevel(ELevelType LevelType)
{
	
	switch (LevelType)
	{
	case ELevelType::None:
		break;
	case ELevelType::DynamicShadow:
		EnablePersistentLightings();
		UnLoadLevelInstance(SimpleGameDataAsset->DynamicShadowLevel);
		break;
	case ELevelType::Water:
		EnablePersistentLightings();
		UnLoadLevelInstance(SimpleGameDataAsset->WaterLevel);
		break;
	default:
		break;
	}
}

void AInGameMode::LoadLevelInstance(TSoftObjectPtr<UWorld> Level)
{
	///////////////////////////////////////////////////////////////////////////////////////////////
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

void AInGameMode::DisablePersistentLightings()
{
	DisabledPersistentActors.Empty();
	//
	TArray<AActor*> totalActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld()->PersistentLevel, AActor::StaticClass(), totalActors);
	for (AActor* actor : totalActors)
	{
		if (SimpleGameDataAsset->bPersistentLevelLightingOff == true)
		{
			ASkyLight* skyLigthActor = Cast<ASkyLight>(actor);
			ALight* lightActor = Cast<ALight>(actor);
			APostProcessVolume* postprocessVolume = Cast<APostProcessVolume>(actor);
			if (postprocessVolume != nullptr && postprocessVolume->bEnabled == true)
			{
				DisabledPersistentActors.Add(postprocessVolume);
				postprocessVolume->bEnabled = false;
				postprocessVolume->SetActorHiddenInGame(true);
			}
			else if (IsValid(lightActor) == true)
			{
				if (lightActor->GetLightComponent() != nullptr && lightActor->GetLightComponent()->bVisible == true)
				{
					DisabledPersistentActors.Add(lightActor);
					lightActor->GetLightComponent()->ToggleVisibility(false);
					lightActor->SetActorHiddenInGame(true);
					lightActor->bEnabled = false;
				}
			}
			else if (IsValid(skyLigthActor) == true)
			{
				if (skyLigthActor->GetLightComponent() != nullptr && skyLigthActor->GetLightComponent()->bVisible == true)
				{
					DisabledPersistentActors.Add(skyLigthActor);
					skyLigthActor->GetLightComponent()->ToggleVisibility(false);
					skyLigthActor->SetActorHiddenInGame(true);
					skyLigthActor->bEnabled = false;
				}
			}
		}
	}
}

void AInGameMode::EnablePersistentLightings()
{
	for (AActor* actor : DisabledPersistentActors)
	{
		ASkyLight* skyLigthActor = Cast<ASkyLight>(actor);
		ALight* lightActor = Cast<ALight>(actor);
		APostProcessVolume* postprocessVolume = Cast<APostProcessVolume>(actor);
		if (postprocessVolume != nullptr)
		{
			postprocessVolume->bEnabled = true;
			postprocessVolume->SetActorHiddenInGame(false);
		}
		else if (IsValid(lightActor) == true)
		{
			if (lightActor->GetLightComponent() != nullptr)
			{
				lightActor->GetLightComponent()->ToggleVisibility(true);
				lightActor->SetActorHiddenInGame(false);
				lightActor->bEnabled = true;
			}
		}
		else if (IsValid(skyLigthActor) == true)
		{
			if (skyLigthActor->GetLightComponent() != nullptr)
			{
				skyLigthActor->GetLightComponent()->ToggleVisibility(true);
				skyLigthActor->SetActorHiddenInGame(false);
				skyLigthActor->bEnabled = true;
			}
		}
	}
}

void AInGameMode::LoadSubLevelStream()
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(GetWorld(), *SimpleGameDataAsset->DynamicShadowLevel.GetAssetName(), true, false, LatentInfo);
	ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(GetWorld(), *SimpleGameDataAsset->DynamicShadowLevel.GetAssetName());
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
	UGameplayStatics::UnloadStreamLevel(GetWorld(), *SimpleGameDataAsset->DynamicShadowLevel.GetAssetName(), LatentInfo, false);
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

		//
		//USkyLightComponent::UpdateSkyCaptureContents(GetWorld());
		//UReflectionCaptureComponent::UpdateReflectionCaptureContents(GetWorld());
		//

		TArray<AActor*> totalActors;
		UGameplayStatics::GetAllActorsOfClass(CurrentLevelStreaming->GetLoadedLevel(), AActor::StaticClass(), totalActors);
		for (AActor* actor : totalActors)
		{
			AGamePlayerCharacter* gamePlayerCharacter = Cast<AGamePlayerCharacter>(actor);
			if (gamePlayerCharacter == nullptr)
			{
				actor->SetActorTickEnabled(false);
				UActorComponent* comp = actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
				/*UStaticMeshComponent* staticMeshComp = Cast<UStaticMeshComponent>(comp);
				if (IsValid(staticMeshComp) == true)
				{
					staticMeshComp->SetCastShadow(false);
				}*/
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 99999.0f, FColor::Yellow, TEXT("gamePlayerCharacter->PossessedBy(GetWorld()->GetFirstPlayerController()..."));
				gamePlayerCharacter->PossessedBy(GetWorld()->GetFirstPlayerController());
			}

			/*ASkyLight* skyLight = Cast<ASkyLight>(actor);
			if (IsValid(skyLight) == true)
			{
				USkyLightComponent* lightComp = skyLight->GetLightComponent();
				if (IsValid(lightComp) == true)
				{
					// recapture는 hitch 유발 가능성이 높음.
					lightComp->RecaptureSky();
				}
			}*/
		}
		
	}
	GetWorld()->FlushLevelStreaming();
}
