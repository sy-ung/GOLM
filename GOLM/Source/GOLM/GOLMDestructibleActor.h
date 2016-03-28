// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GOLMDestructibleActor.generated.h"

UCLASS()
class GOLM_API AGOLMDestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGOLMDestructibleActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = DestructibleComponent)
		UDestructibleComponent *DestructibleComponent;


	virtual void Tick(float DeltaSeconds) override;

};
