// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Runtime/Engine/Classes/Engine/SkyLight.h"
#include "SimpleLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API ASimpleLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Blueprint")
	void CustomLoadStreamLevel(FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad);

	void AfterLoadStreamLevel();

	UPROPERTY(EditDefaultsOnly)
	ASkyLight* SkyLightActor;
};
