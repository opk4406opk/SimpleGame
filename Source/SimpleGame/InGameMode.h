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
	// AA �׽�Ʈ �Լ�.
	UFUNCTION(Exec)
	void TestAAMethod(EGameAntialiasingMethod methodType);
	// Level �ε� �׽�Ʈ.
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void LoadOtherLevel();
	UFUNCTION(BlueprintCallable, Category = "TEST")
	void UnLoadOtherLevel();
private:
	void LoadLevelInstance(TSoftObjectPtr<UWorld> Level);
	void UnLoadLevelInstance(TSoftObjectPtr<UWorld> Level);
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
};
