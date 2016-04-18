// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "GOLMPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API AGOLMPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:

	AGOLMPlayerStart();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpawnArea) UBoxComponent *SpawnArea;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpawnArea) bool bIsRandomSpawn;

	UFUNCTION(BlueprintCallable, Category = Data) FVector GetSpawnLocation();

private:
	
	
};
