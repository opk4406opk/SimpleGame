// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleGameGameModeBase.h"
#include "SimpleGame/GamePlayerCharacter.h"
#include "SimpleGame/GamePlayerController.h"
#include "SimpleGame/GamePlayerCameraManager.h"
#include "SimpleGame/SimpleGameData.h"
#include "Enums.h"
#include "Runtime/Engine/Classes/Engine/LevelStreamingDynamic.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/UObject/ScriptDelegates.h"
#include "SimpleGame/SimpleUserWidget.h"

#include "InGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API AInGameMode : public ASimpleGameGameModeBase
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	// AA 테스트 함수.
	UFUNCTION(Exec)
	void TestAAMethod(EGameAntialiasingMethod methodType);
	// Level 로딩 테스트.
	UFUNCTION(BlueprintCallable, Category = "Level Loading")
	void LoadOtherLevel(ELevelType LevelType);
	UFUNCTION(BlueprintCallable, Category = "Level Loading")
	void UnLoadOtherLevel(ELevelType LevelType);

	UFUNCTION(BlueprintCallable, Category = "TEST_Function")
	void InitSettingExtentWidgets(UWidget* MaxExtent, UWidget* MinExtent);
	UFUNCTION(BlueprintCallable, Category = "TEST_Function")
	void InitSettingActorPivotWidget(UWidget* PivotWidget);

	USimpleUserWidget* GetUserHUDWidget();
	
private:
	void LoadLevelInstance(TSoftObjectPtr<UWorld> Level);
	void UnLoadLevelInstance(TSoftObjectPtr<UWorld> Level);

	void DisablePersistentLightings();
	void EnablePersistentLightings();

	void LoadSubLevelStream();
	void UnLoadSubLevelStream();
	UFUNCTION()
	void OnFinishLoadSubLevel();
public:
	UPROPERTY(EditDefaultsOnly)
	USimpleGameData* SimpleGameDataAsset;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> UserInterfaceWidgetClass;
private:
	UPROPERTY(Transient)
	AGamePlayerCharacter* GamePlayerInstance;
	UPROPERTY(Transient)
	USimpleUserWidget* UserInterfaceWidget;
	UPROPERTY(Transient)
	ULevelStreamingDynamic* CurrentLevelStreaming;
	//
	TScriptDelegate<FWeakObjectPtr> OnFinishLoadSubLevelDelegate;
	//
	UPROPERTY(Transient)
	TArray<AActor*> DisabledPersistentActors;
};
