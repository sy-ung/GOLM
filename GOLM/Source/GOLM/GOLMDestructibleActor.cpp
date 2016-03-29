// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMDestructibleActor.h"


// Sets default values
AGOLMDestructibleActor::AGOLMDestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComponent"));
}

void AGOLMDestructibleActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_PHYSX
	//GEngine->ClearOnScreenDebugMessages();
	//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "I GOT PHYSX BITCH");

	//uint32 ChunkCount = DestructibleComponent->ApexDestructibleActor->getNumVisibleChunks();
	//const uint16 *ChunkIndices = DestructibleComponent->ApexDestructibleActor->getVisibleChunks();
	//for (uint32 i = 0; i < ChunkCount; i++)
	//{
	//	PxRigidDynamic *PxActor = DestructibleComponent->ApexDestructibleActor->getChunkPhysXActor(ChunkIndices[i]);
	//	check(PxActor);
	//	PxTransform Transform = PxActor->getGlobalPose();
	//	PxVec3 &PxLocation = Transform.p;

	//	FVector Location(PxLocation.x, PxLocation.y, PxLocation.z);
	//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Green, "Chunk Location: " + Location.ToString());
	//}

#endif
}


