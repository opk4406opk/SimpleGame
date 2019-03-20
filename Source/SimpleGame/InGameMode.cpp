// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("AInGameMode::InitGame"));
}

void AInGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("AInGameMode::StartPlay"));
	//
	GamePlayer.LoadSynchronous();
	GamePlayerInstance = GetWorld()->SpawnActor<AGamePlayer>(GamePlayer->GetClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
}