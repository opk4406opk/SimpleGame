// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshProjectShadowTargetActor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

// Sets default values
AMeshProjectShadowTargetActor::AMeshProjectShadowTargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UActorComponent* comp = GetComponentByClass(UStaticMeshComponent::StaticClass());
	StaticMeshComponent = Cast<UStaticMeshComponent>(comp);
}

TArray<FVector> AMeshProjectShadowTargetActor::GetVerticesWorldPosition()
{
	TArray<FVector> vertices;
	if (StaticMeshComponent->GetStaticMesh()->RenderData->LODResources.Num() > 0)
	{
		FPositionVertexBuffer* VertexBuffer = &StaticMeshComponent->GetStaticMesh()->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;
		if (VertexBuffer)
		{
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for (int32 Index = 0; Index < VertexCount; Index++)
			{
				//This is in the Static Mesh Actor Class, so it is location and tranform of the SMActor
				const FVector worldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
				//add to output FVector array
				vertices.Add(worldSpaceVertexLocation);
			}
		}
	}
	return vertices;
}

// Called when the game starts or when spawned
void AMeshProjectShadowTargetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshProjectShadowTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

