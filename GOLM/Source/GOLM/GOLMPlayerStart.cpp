// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerStart.h"



AGOLMPlayerStart::AGOLMPlayerStart()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	//SpawnArea->AttachTo(RootComponent);
	SetRootComponent(SpawnArea);
}

FVector AGOLMPlayerStart::GetSpawnLocation()
{
	//if(bIsRandomSpawn)

	FVector NewLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
	//return SpawnArea->GetComponentLocation();
	
	return NewLocation;
}