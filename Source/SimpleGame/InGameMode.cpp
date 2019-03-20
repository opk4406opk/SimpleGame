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
	GamePlayerCharacter.LoadSynchronous();
	GamePlayerInstance = GetWorld()->SpawnActor<AGamePlayerCharacter>(GamePlayerCharacter->GetClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
#if UE_EDITOR
	GamePlayerInstance->SetActorLabel(FString("GamePlayerCharacter_Instance"));
#endif
}