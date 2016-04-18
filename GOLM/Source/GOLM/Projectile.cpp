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

	MuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleParticle"));
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	DeathParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathParticle"));

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

	ProjectileForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialFoce"));

	SetReplicates(true);
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectile, Alive);
	DOREPLIFETIME(AProjectile, CurrentLevelStream);
	DOREPLIFETIME(AProjectile, TargetLocation);
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

		if(!bVTOL)
			BeginParticle();
	}
	bBeginVTOL = false;
	CollisionBox->IgnoreActorWhenMoving(GetInstigator(), true);
	bVTOLStage1Complete = bVTOLStage2Complete = bVTOLStage3Complete = false;
	VTOLStartLocation = GetActorLocation();
}

//***RealViewer refers to player controller and ViewTarget refers to Character
bool AProjectile::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	
	
	//const AGOLMCharacter *TargetCharacter = Cast<AGOLMCharacter>(ViewTarget);

	//if (TargetCharacter->CurrentLevelStream == "LockerRoom")
	//{
	//	if (TargetCharacter == Cast<AGOLMCharacter>(GetInstigator()))
	//		return true;
	//	else
	//	{

	//		return false;
	//	}
	//		
	//}
	//else if (TargetCharacter->CurrentLevelStream == Cast<AGOLMCharacter>(GetInstigator())->CurrentLevelStream)
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	//
	//return false;

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
		if (Alive)
		{
			LifeTime -= DeltaSeconds;
			if (LifeTime <= 0)
			{
				Alive = false;
			}
		}

		if (!Alive)
		{
			if (bExplosive)
			{
				InflictExplosiveDamage();
			}
			OnDeath();
		}
		else if (bIsClusterProjectile)
		{
			if (bVTOL)
			{
				if ((TargetLocation - GetActorLocation()).Size() <= ClusterLaunchDistance && bVTOLStage2Complete)
					OnDeath();
			}
			else
			{
				if ((TargetLocation - GetActorLocation()).Size() <= ClusterLaunchDistance)
					OnDeath();
			}
		}
	}

	if (bVTOL)
	{
		if (bBeginVTOL)
		{
			VTOLMovement(DeltaSeconds);
		}
		else if (!bBeginVTOL)
		{
			if (VTOLStartTimer <= 0)
			{
				bBeginVTOL = true;

				BeginParticle();
					
			}
			VTOLStartTimer -= DeltaSeconds;
		}
	}
}

void AProjectile::VTOLMovement(float DeltaSeconds)
{
	if(!bVTOLStage1Complete)
	{
		if ((GetActorLocation() - VTOLStartLocation).Size() < VTOLHeight)
		{
			
			FQuat ActorForwardQuat = GetActorForwardVector().Rotation().Quaternion();
			FQuat TargetQuat = FVector(0, 0, 1).Rotation().Quaternion();
			FQuat InterpQuat = FMath::LerpStable<FQuat>(ActorForwardQuat, TargetQuat, 0.1);
			
			MovementComponent->Velocity = InterpQuat.Rotator().Vector() * VTOLSpeed;
			
			//MovementComponent->UpdateComponentVelocity();
			
		}
		else
			bVTOLStage1Complete = true;
	}
	else if(!bVTOLStage2Complete)
	{
		FVector VTOLDropStagingLoc = FVector(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + VTOLHeight);
		MovementComponent->Velocity = FMath::Lerp<FRotator>(GetActorForwardVector().Rotation(), (VTOLDropStagingLoc - GetActorLocation()).Rotation(), 0.25f).Vector() * VTOLSpeed;

		if ((VTOLDropStagingLoc - GetActorLocation()).Size() < VTOLDropRadius)
			bVTOLStage2Complete = true;

	}
	else if (!bVTOLStage3Complete)
	{
		MovementComponent->Velocity = FMath::Lerp<FRotator>(GetActorForwardVector().Rotation(), (TargetLocation - GetActorLocation()).Rotation(),1.0f).Vector() * (VTOLSpeed * 1.5);
		
	}
	MovementComponent->UpdateComponentVelocity();
}

void AProjectile::OnDeath()
{
	if (Role == ROLE_Authority)
	{

		if(bIsClusterProjectile)
			FireCluster();

		//ClientOnDeath();
		Destroy();
	}

}


void AProjectile::InflictExplosiveDamage()
{
	if(bExplosive)
	{
		TSubclassOf<UDamageType> DamageType = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		AController *ConInstigator = Cast<AGOLMCharacter>(GetInstigator())->GetController();
		if(ConInstigator != NULL && GetInstigator() != NULL)
		{
			if (bExplosive)
			{
				TArray<AActor*> OverlappedActors;
				UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), ExplosionRadius, DamageType, OverlappedActors, GetInstigator(), ConInstigator);
			}
		}
	}
}




void AProjectile::SetHomingPosition(FVector Location)
{
	
}

void AProjectile::NotifyHit(class UPrimitiveComponent * MyComp, AActor * Other, class UPrimitiveComponent * OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (MovementComponent->bShouldBounce)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MoveSounds->Sound, GetActorLocation());
	}

	if(Role == ROLE_Authority)
	{
		if (!MovementComponent->bShouldBounce)
		{
			Alive = false;

			TSubclassOf<UDamageType> DamageType = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			AController *ConInstigator = Cast<AGOLMCharacter>(GetInstigator())->GetController();

			auto *HitPlayer2 = Cast<AGOLMCharacter>(Other);
			if (HitPlayer2 != GetInstigator())
			{
				if (HitPlayer2 != NULL)
				{
					if (!bExplosive)
						UGameplayStatics::ApplyDamage(Other, Damage, ConInstigator, GetInstigator(), DamageType);
				}
			}
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
			FVector Direction;

			if(!MovementComponent->bShouldBounce)
				Direction = SpreadRandom.VRandCone(MovementComponent->Velocity.GetSafeNormal(), SpreadCone, SpreadCone);
			if (MovementComponent->bShouldBounce)
				Direction = SpreadRandom.VRandCone(FVector::UpVector, SpreadCone, SpreadCone);

			FActorSpawnParameters spawnParams;
			spawnParams.Instigator = Cast<AGOLMCharacter>(GetInstigator());
			AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(ClusterProjectile.GetDefaultObject()->GetClass(), GetActorLocation(), Direction.Rotation(), spawnParams);
			projectile->TargetLocation = TargetLocation;
			projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetInstigator())->CurrentLevelStream;
			projectile->SetActorScale3D(this->GetActorScale3D() * ClusterProjectileScale);
			
		}
		Alive = false;
	}
}

//***Strictly Cosmetics
void  AProjectile::Destroyed()
{
	if (!IsRunningDedicatedServer())
	{

		if (!bIsClusterProjectile)
		{
			if (bExplosive)
			{
				if (ExplosionBP != NULL)
				{

					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					AExplosion *Explode = GetWorld()->SpawnActor<AExplosion>(ExplosionBP.GetDefaultObject()->GetClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
					Explode->ExplosionForce->Radius = ExplosionRadius;
				}
			}
		}


		if (DeathParticle != NULL && !bIsClusterProjectile)
			UGameplayStatics::SpawnEmitterAtLocation(CollisionBox, DeathParticle->Template, this->GetActorLocation(), this->GetActorRotation());

		if (DeathSounds->Sound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSounds->Sound, GetActorLocation());
		}

		if (ProjectileMesh != NULL)
			ProjectileMesh->DestroyComponent();

		if (RearParticle != NULL)
			RearParticle->Deactivate();
		ProjectileForceComponent->Activate();
		ProjectileForceComponent->FireImpulse();
	}



	Super::Destroyed();
}