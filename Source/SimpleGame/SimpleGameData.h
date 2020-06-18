// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimpleGameData.generated.h"

class AGamePlayerCharacter;
class AGamePlayerController;
/**
 * 
 */
UCLASS()
class SIMPLEGAME_API USimpleGameData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly) TSoftObjectPtr<UWorld> DynamicShadowLevel;
	UPROPERTY(EditDefaultsOnly) TSoftObjectPtr<UWorld> WaterLevel;
	UPROPERTY(EditDefaultsOnly) TSoftObjectPtr<UWorld> LightMapLevel;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AGamePlayerCharacter> GamePlayerCharacterClass;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AGamePlayerController> GamePlayerControllerClass;
	// configs
	UPROPERTY(EditDefaultsOnly) bool bPersistentLevelLightingOff = true;
};
