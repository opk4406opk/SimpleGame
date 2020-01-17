// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleLevelScriptActor.h"
#include "Runtime/Engine/Classes/Engine/LatentActionManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void ASimpleLevelScriptActor::CustomLoadStreamLevel(FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad)
{
	FLatentActionInfo actionInfo;
	actionInfo.CallbackTarget = this;
	actionInfo.ExecutionFunction = TEXT("AfterLoadStreamLevel");
	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, bMakeVisibleAfterLoad, bShouldBlockOnLoad, actionInfo);
}

void ASimpleLevelScriptActor::AfterLoadStreamLevel()
{

}
