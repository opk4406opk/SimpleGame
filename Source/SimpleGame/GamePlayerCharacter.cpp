// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerCharacter.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"

// Sets default values
AGamePlayerCharacter::AGamePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

}

// Called when the game starts or when spawned
void AGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("AGamePlayerCharacter::BeginPlay"));
	CameraComponent = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));

	
}

// Called every frame
void AGamePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FRotator curRotation = GetActorRotation();
	//SetActorRotation(curRotation + LatestTouchRotation * RotateSpeed);
	//
	OrbitRotation();
}

// Called to bind functionality to input
void AGamePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveTouchPadX", this, &AGamePlayerCharacter::MoveTouchPadX);
	PlayerInputComponent->BindAxis("MoveTouchPadY", this, &AGamePlayerCharacter::MoveTouchPadY);
	PlayerInputComponent->BindAxis("RotationTouchPadX", this, &AGamePlayerCharacter::RotationTouchPadX);
	PlayerInputComponent->BindAxis("RotationTouchPadY", this, &AGamePlayerCharacter::RotationOtuchPadY);
}

void AGamePlayerCharacter::MoveTouchPadX(float x)
{
	UE_LOG(LogTemp, Display, TEXT("AGamePlayerCharacter::MoveTouchPadX : %f"), x);
	FVector curLoc = GetActorLocation();
	curLoc.X -= x * MoveSpeed;
	SetActorLocation(curLoc);
}

void AGamePlayerCharacter::MoveTouchPadY(float y)
{
	UE_LOG(LogTemp, Display, TEXT("AGamePlayerCharacter::MoveTouchPadY : %f"), y);
	FVector curLoc = GetActorLocation();
	curLoc.Y += y * MoveSpeed;
	SetActorLocation(curLoc);
}

void AGamePlayerCharacter::RotationTouchPadX(float x)
{
	UE_LOG(LogTemp, Display, TEXT("AGamePlayerCharacter::RotationTouchPadX : %f"), x);
	LatestTouchRotation.Yaw = x;
}

void AGamePlayerCharacter::RotationOtuchPadY(float y)
{
	UE_LOG(LogTemp, Display, TEXT("AGamePlayerCharacter::RotationOtuchPadY : %f"), y);
	//LatestTouchRotation.Roll = y;
	if (CameraComponent == nullptr) return;
	CameraComponent->AddRelativeRotation(FRotator(y, 0.0f, 0.0f));
}

void AGamePlayerCharacter::OrbitRotation()
{
	if (IsValid(TargetActor) == true)
	{
		FVector InitPlayerWorldLocation = GetActorLocation();

		//FVector TargetToPlayerDirection = InitPlayerWorldLocation - TargetActor->GetActorLocation();
		//TargetToPlayerDirection.Normalize();
		FVector PlayerToTargetDirection = TargetActor->GetActorLocation() - InitPlayerWorldLocation;
		PlayerToTargetDirection.Normalize();
		const FVector RotatedVector = PlayerOrbitRotator.RotateVector(PlayerToTargetDirection * OrbitRotateSpeed);
		const FVector newWorldLocation = InitPlayerWorldLocation + RotatedVector;
		SetActorLocation(newWorldLocation);
		//SetActorRotation((TargetActor->GetActorLocation() - newWorldLocation).Rotation());
	}
}

