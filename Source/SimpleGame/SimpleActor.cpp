// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleActor.h"
#include "InGameMode.h"
#include "SimpleUserWidget.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Runtime/Engine/Public/UnrealEngine.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

// Sets default values
ASimpleActor::ASimpleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DefaultMeshComp"));
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DefaultCapsuleComp"));
	CapsuleComponent->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleComponent->SetCapsuleHalfHeight(90.0f);
	CapsuleComponent->SetCapsuleRadius(30.0f);
	//
	FVector offsetedLoc = CapsuleComponent->GetComponentTransform().GetLocation();
	offsetedLoc.Z += 100.0f;
	CapsuleComponent->SetWorldLocation(offsetedLoc);
}

// Called when the game starts or when spawned
void ASimpleActor::BeginPlay()
{
	Super::BeginPlay();
	bTicking = true;
}

void ASimpleActor::OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{

}

UAnimMontage* ASimpleActor::PlayAnimationSequence(UAnimSequenceBase* Asset, float BlendInTime /*= 0.25f*/, float BlendOutTime /*= 0.25f*/, float InPlayRate /*= 1.f*/, int32 LoopCount /*= 1*/, float InTimeToStartMontageAt /*= 0.f*/)
{
	check(MeshComponent != nullptr);

	float blendOutTriggerTime = -1.0f;

	// BlendOutTime이 0보다 클 경우 Blend out을 해야 하는데, 그때는 Anim이 끝난 이후 진행되게
	if (BlendOutTime > 0.0f)
	{
		blendOutTriggerTime = 0.0f;
	}

	UAnimMontage* montage = nullptr;
	if (MeshComponent->GetAnimInstance() != nullptr)
	{
		montage = MeshComponent->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(Asset, FAnimSlotGroup::DefaultSlotName, BlendInTime, BlendOutTime, InPlayRate, LoopCount, blendOutTriggerTime, InTimeToStartMontageAt);
	}
	// blend out time이 0 이하일때는 blend out을 하지 않는다는 의미이므로 바로 EndDelegate에 연결
	// 아니면 BlendOut되는 시점을 anim end시점으로 설정
// 	if(BlendOutTime <= 0.0f)
// 	{
// 		FOnMontageEnded animDelegate;
// 		animDelegate.BindUObject(this, &ALobbyCreature::OnAnimationEnded);
// 		MeshComponent->GetAnimInstance()->Montage_SetEndDelegate(animDelegate, montage);
// 	}
// 	else
	{
		if (MeshComponent->GetAnimInstance() != nullptr)
		{
			FOnMontageBlendingOutStarted animDelegate;
			animDelegate.BindUObject(this, &ASimpleActor::OnAnimationEnded);
			MeshComponent->GetAnimInstance()->Montage_SetBlendingOutDelegate(animDelegate, montage);
		}
	}

	return montage;
}

// Called every frame
void ASimpleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTicking == false) return;

	if (GEngine)
	{
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		float mouseScreenX, mouseScreenY;
		const bool bValid = playerController->GetMousePosition(mouseScreenX, mouseScreenY);
		if (bValid == true)
		{
			GEngine->ClearOnScreenDebugMessages();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Mouse Screen Location is: X : %d, Y : %d"), mouseScreenX, mouseScreenY));
		
			int32 viewportSizeX, viewportSizeY;
			playerController->GetViewportSize(viewportSizeX, viewportSizeY);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Viewport: X : %d, Y : %d"), viewportSizeX, viewportSizeY));
		}
	}
	//
	AInGameMode* ingameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(ingameMode) == true && IsValid(CapsuleComponent) == true)
	{
		FBoxSphereBounds sphereBounds = CapsuleComponent->CalcBounds(GetTransform());
		const FVector halfBoxBound = sphereBounds.GetBox().GetSize() / 2;
		const FVector originActorLocation = GetTransform().GetLocation();
		FVector minExtent = FVector(originActorLocation.X, originActorLocation.Y - halfBoxBound.Y, originActorLocation.Z);
		FVector maxExtent = FVector(originActorLocation.X, originActorLocation.Y + halfBoxBound.Y, originActorLocation.Z + 180.0f);
		//
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		USimpleUserWidget* hud = ingameMode->GetUserHUDWidget();
		if (IsValid(hud) == true)
		{
			
			if (IsValid(hud->MaxExtentWidget) == true)
			{
				const float viewportScale = UWidgetLayoutLibrary::GetViewportScale(hud->MaxExtentWidget);
				FVector2D maxExtentScreenPos;
				playerController->ProjectWorldLocationToScreen(maxExtent, maxExtentScreenPos);
				//FVector2D absPos = hud->MaxExtentWidget->GetCachedGeometry().LocalToAbsolute(maxExtentScreenPos);
				UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(hud->MaxExtentWidget->Slot);
				maxExtentScreenPos.X /= viewportScale;
				maxExtentScreenPos.Y /= viewportScale;
				//
				FAnchors zeroAnchor;
				zeroAnchor.Minimum = FVector2D::ZeroVector;
				zeroAnchor.Maximum = FVector2D::ZeroVector;
				panelSlot->SetAnchors(zeroAnchor);
				panelSlot->SetPosition(maxExtentScreenPos);
				
			}

			if (IsValid(hud->MinExtentWidget) == true)
			{
				const float viewportScale = UWidgetLayoutLibrary::GetViewportScale(hud->MinExtentWidget);
				FVector2D minExtentScreenPos;
				playerController->ProjectWorldLocationToScreen(minExtent, minExtentScreenPos);
				//FVector2D absPos = hud->MinExtentWidget->GetCachedGeometry().LocalToAbsolute(minExtentScreenPos);
				UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(hud->MinExtentWidget->Slot);
				minExtentScreenPos.X /= viewportScale;
				minExtentScreenPos.Y /= viewportScale;
				//
				FAnchors zeroAnchor;
				zeroAnchor.Minimum = FVector2D::ZeroVector;
				zeroAnchor.Maximum = FVector2D::ZeroVector;
				panelSlot->SetAnchors(zeroAnchor);
				panelSlot->SetPosition(minExtentScreenPos);
			}

			if (IsValid(hud->PivotWidget) == true)
			{
				const float viewportScale = UWidgetLayoutLibrary::GetViewportScale(hud->PivotWidget);
				FVector2D pivotScreenPos;
				playerController->ProjectWorldLocationToScreen(GetTransform().GetLocation(), pivotScreenPos);
				//FVector2D absPos = hud->PivotWidget->GetCachedGeometry().LocalToAbsolute(pivotScreenPos);
				UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(hud->PivotWidget->Slot);
				pivotScreenPos.X /= viewportScale;
				pivotScreenPos.Y /= viewportScale;
				//
				FAnchors zeroAnchor;
				zeroAnchor.Minimum = FVector2D::ZeroVector;
				zeroAnchor.Maximum = FVector2D::ZeroVector;
				panelSlot->SetAnchors(zeroAnchor);
				panelSlot->SetPosition(pivotScreenPos);
			}

		}
	}
}

void ASimpleActor::RotateActor()
{
	if (bLocalSpace == true)
	{
		FVector RotatedVec = Rotator.RotateVector(GetActorUpVector());
		SetActorRotation(RotatedVec.Rotation());
	}
	else
	{
		FVector RotatedVec = Rotator.RotateVector(GetActorUpVector());
		MeshComponent->SetWorldRotation(RotatedVec.Rotation());
	}
	
}

