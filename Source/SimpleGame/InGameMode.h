// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleGameGameModeBase.h"
#include "SimpleGame/GamePlayerCharacter.h"
#include "SimpleGame/GamePlayerController.h"
#include "SimpleGame/GamePlayerCameraManager.h"
#include "Enums.h"
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

	UFUNCTION(Exec) void TestAAMethod(EGameAntialiasingMethod methodType);
public:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AGamePlayerCharacter> GamePlayerCharacter;

private:
	UPROPERTY()
	AGamePlayerCharacter* GamePlayerInstance;

};
