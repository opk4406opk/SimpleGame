// Fill out your copyright notice in the Description page of Project Settings.


#include "KojeomGeneralActor.h"

// Sets default values
AKojeomGeneralActor::AKojeomGeneralActor()
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
}

// Called when the game starts or when spawned
void AKojeomGeneralActor::BeginPlay()
{
	Super::BeginPlay();
	
	//MeshComponent->SetSkeletalMesh(GeneralActorData.MeshAsset.LoadSynchronous());
	//MeshComponent->SetAnimInstanceClass(GeneralActorData.AnimClass.LoadSynchronous());
}

// Called every frame
void AKojeomGeneralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

