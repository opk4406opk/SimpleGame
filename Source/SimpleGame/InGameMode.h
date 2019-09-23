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
	// AA 테스트 함수.
	UFUNCTION(Exec)
	void TestAAMethod(EGameAntialiasingMethod methodType);
	// Level 로딩 테스트.
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void LoadOtherLevel();
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void UnLoadOtherLevel();
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void LoadSimpleLevelWithLS(bool IncludeLS);
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void UnLoadSimpleLevelWithLS(bool IncludeLS);
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void LoadSubStreamOtherLevel();
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void UnLoadSubStreamOtherLevel();
private:
	void LoadLevelInstance(TSoftObjectPtr<UWorld> Level);
	void UnLoadLevelInstance(TSoftObjectPtr<UWorld> Level);

	void LoadSubLevelStream();
	void UnLoadSubLevelStream();
public:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AGamePlayerCharacter> GamePlayerCharacter;
	UPROPERTY(EditDefaultsOnly)
	USimpleGameData* SimpleGameDataAsset;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> UserInterfaceWidgetClass;
private:
	UPROPERTY(Transient)
	AGamePlayerCharacter* GamePlayerInstance;
	UPROPERTY(Transient)
	UUserWidget* UserInterfaceWidget;
	UPROPERTY(Transient)
	ULevelStreamingDynamic* LevelStreaming;
};
