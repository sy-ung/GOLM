// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameMode.h"
#include "GOLMLevelStreamBeacon.h"


// Sets default values
AGOLMLevelStreamBeacon::AGOLMLevelStreamBeacon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	GlowingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Glow"));
	GlowingEffect->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AGOLMLevelStreamBeacon::BeginPlay()
{
	Super::BeginPlay();
	if (GlowingEffect != NULL)
	{
		UGameplayStatics::SpawnEmitterAttached(GlowingEffect->Template, CollisionBox);
	}
}

// Called every frame
void AGOLMLevelStreamBeacon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGOLMLevelStreamBeacon::NotifyHit(
	class UPrimitiveComponent * MyComp, AActor * Other, class UPrimitiveComponent * OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Role == ROLE_Authority)
	{
		AGOLMCharacter *Player = Cast<AGOLMCharacter>(Other);
		if (Player != NULL)
		{
			Player->LoadEntranceLevel(EntranceLevelName, ExitLevelName);
		}
	}
}