// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GOLMCharacter.h"
#include "GOLMLevelStreamBeacon.generated.h"

UCLASS()
class GOLM_API AGOLMLevelStreamBeacon : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGOLMLevelStreamBeacon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		virtual void NotifyHit
		(class UPrimitiveComponent * MyComp,	AActor * Other, class UPrimitiveComponent * OtherComp, bool bSelfMoved,
		FVector HitLocation,FVector HitNormal,FVector NormalImpulse,const FHitResult & Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerStartLocations)
		FName EntranceLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerStartLocations)
		FName ExitLevelName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aesthetics)
		UParticleSystemComponent *GlowingEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		UBoxComponent *CollisionBox;




};
