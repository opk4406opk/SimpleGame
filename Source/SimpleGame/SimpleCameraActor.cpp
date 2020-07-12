// Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "SimpleCameraActor.h"

void ASimpleCameraActor::BeginPlay()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	EnableInput(playerController);

	InputComponent->BindTouch(IE_Pressed, this, &ASimpleCameraActor::OnTouchBegin);
	InputComponent->BindTouch(IE_Released, this, &ASimpleCameraActor::OnTouchEnd);
	InputComponent->BindTouch(IE_Repeat, this, &ASimpleCameraActor::OnFingerMove);


	playerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
}

void ASimpleCameraActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void ASimpleCameraActor::OnTouchBegin(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		LastTouchLocation = FVector2D(Location);
		StartTouchLocation = LastTouchLocation;
	}
}

void ASimpleCameraActor::OnTouchEnd(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		EndTouchLocation = FVector2D::ZeroVector;
		LastTouchLocation = EndTouchLocation;
	}
}

static const float TouchDragRotationScale = 0.1f;

void ASimpleCameraActor::OnFingerMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	if ((FingerIndex == ETouchIndex::Touch1) && (LastTouchLocation.IsZero() == false))
	{
		const FVector2D direction = (FVector2D(Location) - LastTouchLocation).GetSafeNormal();
		LastDirection = direction;
		const FVector2D dragDelta = (FVector2D(Location) - LastTouchLocation) * TouchDragRotationScale;
		const float screenSwipeLength = (FVector2D(Location) - StartTouchLocation).Size();
		const float lerpValue = FMath::Clamp(screenSwipeLength / ViewportSizeX, 0.0f, 1.0f);
		

		const FVector moveValue = FVector(40.0f * direction.X, 40.0f * direction.Y, 0.0f);
		const FVector targetPos = GetActorLocation() + moveValue;
		FVector newPos = FMath::Lerp(GetActorLocation(), targetPos, lerpValue);
		SetActorLocation(targetPos);

		UE_LOG(LogTemp, Log, TEXT("ASimpleCameraActor::OnFingerMove swipe length = (%f), drag delta X : (%f) Y : (%f), Lerp Value : (%f)"), 
			screenSwipeLength, dragDelta.X, dragDelta.Y, lerpValue);

		LastTouchLocation = FVector2D(Location);

	}
}
