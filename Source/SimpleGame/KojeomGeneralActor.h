// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "KojeomGeneralActor.generated.h"

UCLASS()
class SIMPLEGAME_API AKojeomGeneralActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKojeomGeneralActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(Transient) USkeletalMeshComponent* MeshComponent = nullptr;
	UPROPERTY(Transient) UCapsuleComponent* CapsuleComponent = nullptr;
};
