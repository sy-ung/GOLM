// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerStart.h"



AGOLMPlayerStart::AGOLMPlayerStart()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");
	SetRootComponent(SpawnArea);
}

FVector AGOLMPlayerStart::GetSpawnLocation()
{
	if(bIsRandomSpawn)
		return UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);

	return GetActorLocation();

}