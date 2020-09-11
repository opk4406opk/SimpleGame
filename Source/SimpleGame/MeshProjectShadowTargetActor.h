// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshProjectShadowTargetActor.generated.h"

UCLASS()
class SIMPLEGAME_API AMeshProjectShadowTargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshProjectShadowTargetActor();
public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) 
	TArray<FVector> GetVerticesWorldPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(Transient, VisibleInstanceOnly) UStaticMeshComponent* StaticMeshComponent = nullptr;
};
