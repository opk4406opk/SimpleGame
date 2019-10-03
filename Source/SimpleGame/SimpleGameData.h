// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimpleGameData.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API USimpleGameData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> OhterLevel;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> SimpleLevel;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MidiumLevel;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> BigLargeLevel;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LS_SimpleLevel;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultInGameLevel;

};
