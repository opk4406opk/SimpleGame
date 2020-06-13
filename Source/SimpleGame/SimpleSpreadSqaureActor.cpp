// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleSpreadSqaureActor.h"

// Sets default values
ASimpleSpreadSqaureActor::ASimpleSpreadSqaureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void ASimpleSpreadSqaureActor::BeginPlay()
{
	Super::BeginPlay();
	//InitScale = FVector(1.0f, 1.0f, 1.0f);
	//FinalScale = FVector(10.0f, 10.0, 1.0f);
	// coll setting.
	StaticMeshComponent->SetStaticMesh(ActorStaticMesh);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetNotifyRigidBodyCollision(true); // 물리 이벤트.
	StaticMeshComponent->SetGenerateOverlapEvents(true);  // 트리거 이벤트
}

// Called every frame
void ASimpleSpreadSqaureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SumDeltaTime += DeltaTime;
	if (SumDeltaTime >= 2.5f)
	{
		SetActorScale3D(InitScale);
		SumDeltaTime = 0.0f;
	}
	//
	FVector lerp = FMath::Lerp(InitScale, FinalScale, SumDeltaTime);
	SetActorScale3D(lerp);
}

void ASimpleSpreadSqaureActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ASimpleSpreadSqaureActor::NotifyActorBeginOverlap"));
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("NotifyActorBeginOverlap, triggered with %s"), *OtherActor->GetActorLabel());
	}
#endif
}

void ASimpleSpreadSqaureActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ASimpleSpreadSqaureActor::NotifyActorEndOverlap"));
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("NotifyActorEndOverlap, triggered with %s"), *OtherActor->GetActorLabel());
	}
#endif
}

void ASimpleSpreadSqaureActor::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ASimpleSpreadSqaureActor::NotifyHit"));
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("NotifyHit, collide with %s"), *Other->GetActorLabel());
	}
#endif
}

