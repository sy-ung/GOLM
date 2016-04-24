// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereBox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereBox"));
	ExplosionEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEmitter"));
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForce"));

	SetRootComponent(SphereBox);
	ExplosionEmitter->AttachTo(GetRootComponent());
	ExplosionSound->AttachTo(GetRootComponent());
	ExplosionForce->AttachTo(GetRootComponent());

	ParticleFinishedDelegate.BindUFunction(this,FName("DestroyMe"));
	ExplosionEmitter->OnSystemFinished.Add(ParticleFinishedDelegate);
	bNetUseOwnerRelevancy = true;
	bAppliedForce = false;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	PlayEffects();
	
}

void AExplosion::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEmitter->Template, GetActorLocation())->SetRelativeScale3D(GetActorScale3D());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound->Sound, GetActorLocation());
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if(UKismetSystemLibrary::IsValid(ExplosionForce))
		ApplyImpulseForce();

	UKismetSystemLibrary::Delay(GetWorld(), 0.032f, FLatentActionInfo());
	if (UKismetSystemLibrary::IsValid(ExplosionForce))
		ExplosionForce->DestroyComponent();
}

void AExplosion::DestroyMe()
{
	Destroy();
}

void AExplosion::ApplyImpulseForce()
{
	ExplosionForce->FireImpulse();
}