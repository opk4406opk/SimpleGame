// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "SimpleSpreadSqaureActor.generated.h"

UCLASS()
class SIMPLEGAME_API ASimpleSpreadSqaureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleSpreadSqaureActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(Transient, EditAnywhere) UStaticMesh* ActorStaticMesh;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; // 트리거 이벤트.
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override; // 트리거 이벤트.
	// 물리를 통한 Hitting 노티파이.
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
private:
	UPROPERTY(Transient, EditDefaultsOnly) UStaticMeshComponent* StaticMeshComponent;
	//
	UPROPERTY(EditDefaultsOnly)
	FVector InitScale = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly)
	FVector FinalScale = FVector::ZeroVector;
	float SumDeltaTime;
};
