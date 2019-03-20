// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleGameGameModeBase.h"

void ASimpleGameGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("ASimpleGameGameModeBase::InitGame"));
}

