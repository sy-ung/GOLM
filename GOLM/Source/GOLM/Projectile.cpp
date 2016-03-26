// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMPlayerController.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	RearParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RearParticle"));
	DeathParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathParticle"));
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	RootComponent = CollisionBox;
	
	MoveSounds = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementSounds"));
	DeathSounds = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathSounds"));
	ProjectileMesh->AttachTo(RootComponent);
	MoveSounds->AttachTo(RootComponent);
	DeathSounds->AttachTo(RootComponent);

	Direction = FVector::ForwardVector;
	CollisionBox->IgnoreActorWhenMoving(Instigator, true);

	ExplosiveSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosiveDamageRadius"));
	ExplosiveSphere->AttachTo(RootComponent);

	SetReplicates(true);
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectile, Alive);
	DOREPLIFETIME(AProjectile, CurrentLevelStream);
}



// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->SetCollisionProfileName("Projectile");
	ProjectileMesh->SetCollisionProfileName("NoCollision");

	//MovementComponent->InitialSpeed = GetInstigator()->GetVelocity().Size() + Speed;
	if(Role == ROLE_Authority)
	{
		Alive = true;
		BeginParticle();
	}
	CollisionBox->IgnoreActorWhenMoving(GetInstigator(), true);
}

//***RealViewer refers to player controller and ViewTarget refers to Character
bool AProjectile::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	

	const AGOLMCharacter *TargetCharacter = Cast<AGOLMCharacter>(ViewTarget);

	if (TargetCharacter->CurrentLevelStream == "LockerRoom")
	{
		if (TargetCharacter == Cast<AGOLMCharacter>(Instigator))
			return true;
		else
			return false;
	}
	else if (TargetCharacter->CurrentLevelStream == Cast<AGOLMCharacter>(Instigator)->CurrentLevelStream)
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	
	return false;

}


void AProjectile::BeginParticle()
{
	if(!IsRunningDedicatedServer())
	{
		if (RearParticle != NULL)
		{
			UGameplayStatics::SpawnEmitterAttached(RearParticle->Template, CollisionBox);
		}
		if (MoveSounds->Sound != NULL)
			MoveSounds->Play();
	}
	if (Role == ROLE_Authority)
		ClientBeginParticle();
}

void AProjectile::ClientBeginParticle_Implementation()
{
	if(Role != ROLE_Authority)
		BeginParticle();
}

// Called every frame
void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Role == ROLE_Authority)
	{
		if(Alive)
		{
			LifeTime -= DeltaSeconds;
			if (LifeTime <= 0)
			{
				Alive = false;
			}
		}
	}

	if (!Alive)
	{
		{
			OnDeath();
		}
	}

	if (GetInstigator() != NULL)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, GetInstigator()->GetName());
	else
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "NO INSTIGATOR");
	
}

void AProjectile::OnDeath()
{


	if(!IsRunningDedicatedServer())
	{
		if (DeathParticle != NULL && !bIsClusterProjectile)
			UGameplayStatics::SpawnEmitterAtLocation(CollisionBox, DeathParticle->Template, this->GetActorLocation(), this->GetActorRotation());

		if (DeathSounds->Sound != NULL)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSounds->Sound, GetActorLocation());

		if (ProjectileMesh != NULL)
			ProjectileMesh->DestroyComponent();

		if (RearParticle != NULL)
			RearParticle->Deactivate();
	}

	if (Role == ROLE_Authority)
	{
		if (bIsClusterProjectile)
			FireCluster();

		InflictDamage();
		
		DestroyMe();
	}
	
}

void AProjectile::ClientOnDeath_Implementation()
{
	if(Role != ROLE_Authority)
		OnDeath();
}

void AProjectile::InflictDamage()
{
	if(bExplosive)
	{
		TArray<AActor*> OverlappingActors;
		ExplosiveSphere->GetOverlappingActors(OverlappingActors);
		for (int32 i = 0; i < OverlappingActors.Num(); i++)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, OverlappingActors[i]->GetName() + " is overlapped");
		}
	}
}

void AProjectile::DestroyMe()
{
		Destroy(true);
}

void AProjectile::ServerDestroyMe_Implementation()
{
	DestroyMe();
}

bool AProjectile::ServerDestroyMe_Validate()
{
	return true;
}

void AProjectile::NotifyHit(class UPrimitiveComponent * MyComp, AActor * Other, class UPrimitiveComponent * OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Alive = false;
	
	if(Role == ROLE_Authority)
	{	
		auto *HitPlayer = Cast<AGOLMCharacter>(Other);
		if (HitPlayer != GetInstigator())
		{
			if(HitPlayer != NULL)
				HitPlayer->Health -= 33;
		}
	}
}


void AProjectile::ToggleNoPawnCollision()
{
	CollisionBox->SetCollisionProfileName("Projectile");
}
void AProjectile::FireCluster()
{
	if(Role == ROLE_Authority)
	{
		for (int32 i = 0; i < NumberInCluster; i++)
		{
			int32 RandomSeed = FMath::Rand();
			FRandomStream SpreadRandom(RandomSeed);
			float SpreadCone = FMath::DegreesToRadians(ClusterSpreadAmount * 0.5);
			FVector Direction = SpreadRandom.VRandCone(GetActorRotation().Vector(), SpreadCone, SpreadCone);
			FActorSpawnParameters spawnParams;
			spawnParams.Instigator = Cast<AGOLMCharacter>(GetInstigator());
			AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(ClusterProjectile.GetDefaultObject()->GetClass(), GetActorLocation(), Direction.Rotation(), spawnParams);
			projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetInstigator())->CurrentLevelStream;
			projectile->SetActorScale3D(this->GetActorScale3D());

		}
	}
}