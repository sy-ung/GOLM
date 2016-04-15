// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class GOLM_API AExplosion : public AActor
{
	GENERATED_BODY()

private:
	FScriptDelegate ParticleFinishedDelegate;

	bool bAppliedForce;

	UFUNCTION()
	void DestroyMe();

public:	
	// Sets default values for this actor's properties
	AExplosion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RootComponent)
		USphereComponent *SphereBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Costmetics)
		UParticleSystemComponent *ExplosionEmitter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Costmetics)
		UAudioComponent *ExplosionSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		URadialForceComponent *ExplosionForce;
	


	void PlayEffects();

	void ApplyImpulseForce();
	
	
};
