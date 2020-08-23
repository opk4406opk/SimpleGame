// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleGameGameModeBase.h"
#include "SimpleStartSceneMode.generated.h"

class USimpleUserWidget;
class UUserWidget;
/**
 * 
 */
UCLASS()
class SIMPLEGAME_API ASimpleStartSceneMode : public ASimpleGameGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
public:
	UFUNCTION(BlueprintCallable, Category = "StartScene") void OnClickStartGame();

public:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> UserInterfaceWidgetClass;
	UPROPERTY(Transient) USimpleUserWidget* UserInterfaceWidget;
};
