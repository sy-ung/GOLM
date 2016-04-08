// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);

	WeaponProjectileTraceImpact = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponProjectileTraceImpact"));
	WeaponProjectileTraceImpact->AttachTo(WeaponMesh,"MuzzleFlash");
	
	WeaponAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponAudio"));
	WeaponAudio->AttachTo(RootComponent);
	
	
	bReplicates = true;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh->SetCollisionProfileName("NoCollision");
	CollisionComp->SetCollisionProfileName("NoCollision");
	if (Role == ROLE_Authority)
	{
		bAbleToShoot = true;
		CurrentProjectile = CompatibleProjectiles[0].GetDefaultObject();
	}
		
	//UGameplayStatics::SpawnEmitterAttached(WeaponProjectileTraceImpact->Template, WeaponMesh, "MuzzleFlash");
}

// Called every frame
void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if(bAutomatic)
	//{
	//	if(WeaponAudio->IsPlaying())
	//	{
	//		if (bStartShooting)
	//		{
	//			WeaponAudio->Stop();
	//			UGameplayStatics::SpawnSoundAtLocation(WeaponMesh, WeaponAutoMaticEnd, WeaponMesh->GetSocketLocation("MuzzleFlash"));
	//		}
	//	}
	//}

	if(Role == ROLE_Authority)
	{

		if (!bAbleToShoot)
		{
			if (TimeBeforeNextShot <= 0)
			{
				bAbleToShoot = true;
			}
			else
				TimeBeforeNextShot -= DeltaSeconds;
		}
	}


	if(bStartShooting)
	{
		if(bAbleToShoot)
			FireWeapon();
	}
	else
	{
		if(bAutomatic)
			if(WeaponAudio->Sound != NULL)
				if (WeaponAudio->IsPlaying())
					WeaponAudio->Stop();
	}
	
	
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, CurrentProjectile);
	DOREPLIFETIME(AWeapon, TargetLocation);
	DOREPLIFETIME(AWeapon, bAbleToShoot);
}




void AWeapon::FireWeapon()
{
	//if(Role!= ROLE_Authority)
	{
		//if(bAbleToShoot)
		{
			if (!bAutomatic)
			{
				bStartShooting = false;
			}
			bAbleToShoot = false;
			if(Role != ROLE_Authority)
				ServerFireWeapon();
		}
	}
	if (Role == ROLE_Authority)
	{
		bAbleToShoot = false;
		TimeBeforeNextShot = FiringRate;
		FRotator WeaponMuzzleRotation = WeaponMesh->GetSocketRotation("MuzzleFlash");
		FVector WeaponMuzzleLocation = WeaponMesh->GetSocketLocation("MuzzleFlash");
		LaunchProjectile(WeaponMuzzleLocation, WeaponMuzzleRotation);
	}
	
}
void AWeapon::ServerFireWeapon_Implementation()
{
	FireWeapon();
}
bool AWeapon::ServerFireWeapon_Validate()
{
	return true;
}





void AWeapon::LaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	PlayLaunchEffects();

	if(Role == ROLE_Authority)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Instigator = Cast<AGOLMCharacter>(GetOwner());
		
		if (bIsSpread)
		{
			LaunchSpreadProjectile(MuzzleLocation, MuzzleRotation);
		}
		else
		{
			AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(CurrentProjectile->GetClass(),MuzzleLocation,MuzzleRotation,spawnParams);

			projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetOwner())->CurrentLevelStream;
			projectile->SetActorScale3D(this->GetActorScale3D());
			projectile->TargetLocation = TargetLocation;
		}
		
	}

	if (Role != ROLE_Authority)
	{
		ServerLaunchProjectile(MuzzleLocation, MuzzleRotation);
	}
		
}

void AWeapon::ServerLaunchProjectile_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	LaunchProjectile(MuzzleLocation, MuzzleRotation);
}

bool AWeapon::ServerLaunchProjectile_Validate(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	return true;
}

void AWeapon::LaunchSpreadProjectile(FVector MuzzleLocation, FRotator MuzzleRotation)
{

	if(Role == ROLE_Authority)
	{
		for (int32 i = 0; i < NumberOfSpread;i++)
		{
		
			int32 RandomSeed = FMath::Rand();
			FRandomStream SpreadRandom(RandomSeed);
			float SpreadCone = FMath::DegreesToRadians(SpreadAmount * 0.5);
			FVector Direction = SpreadRandom.VRandCone(MuzzleRotation.Vector(), SpreadCone, SpreadCone);
			FActorSpawnParameters spawnParams;
			spawnParams.Instigator = Cast<AGOLMCharacter>(GetOwner());
			AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(CurrentProjectile->GetClass(), MuzzleLocation, Direction.Rotation(), spawnParams);
			projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetOwner())->CurrentLevelStream;
			projectile->SetActorScale3D(this->GetActorScale3D());

		}
	}

}

void AWeapon::PlayLaunchEffects()
{
	FVector WMFL = WeaponMesh->GetSocketLocation("MuzzleFlash");
	FRotator WMFR = WeaponMesh->GetSocketRotation("MuzzleFlash");

	if(CurrentProjectile->MuzzleParticle->Template != NULL)
		UGameplayStatics::SpawnEmitterAtLocation(WeaponMesh, CurrentProjectile->MuzzleParticle->Template, WMFL, WMFR);

	//if(CurrentProjectile->FiringSounds != NULL)
	{
		if (bAutomatic)
		{
			if (!WeaponAudio->IsPlaying())
			{
				WeaponAudio->Play();
			}
			
		}
		else
			WeaponAudio->Play();
	}

	
	if(Role == ROLE_Authority)
		ClientPlayLaunchEffects();
}

void AWeapon::ClientPlayLaunchEffects_Implementation()
{
	if(Role != ROLE_Authority)
		PlayLaunchEffects();
}

void AWeapon::Death()
{
	Destroy();
}

void AWeapon::SetRagDoll(bool value)
{
	//WeaponMesh->SetAllBodiesSimulatePhysics(value);
	WeaponMesh->SetSimulatePhysics(value);
	if(value)
	{
		WeaponMesh->SetCollisionProfileName("Ragdoll");
		//WeaponMesh->WakeAllRigidBodies();
	}
	else
	{
		WeaponMesh->SetCollisionProfileName("NoCollision");
		//WeaponMesh->PutAllRigidBodiesToSleep();
	}
}

void AWeapon::ToggleProjectileCollision(bool CollisionPossible)
{
	bProjectileCollisionPossible = CollisionPossible;
}

void AWeapon::SetNewProjectile(AProjectile *NewProjectile)
{
	if (Role == ROLE_Authority)
		CurrentProjectile = NewProjectile;
	else
		ServerSetNewProjectile(NewProjectile);
}

void AWeapon::ServerSetNewProjectile_Implementation(AProjectile *NewProjectile)
{
	SetNewProjectile(NewProjectile);
}

bool AWeapon::ServerSetNewProjectile_Validate(AProjectile *NewProjectile)
{
	return true;
}

float AWeapon::CalculateProjectilePath(FVector TargetPoint)
{
	
	if (Role != ROLE_Authority)
	{
		ServerSetTargetLocation(TargetPoint);
	}
	else
	{
		TargetLocation = TargetPoint;
	}

	

	if (CurrentProjectile->bVTOL)
		return 20.0f;

	FVector SockLoc = WeaponMesh->GetSocketLocation("MuzzleFlash");
	

	float Speed = 0;

	Speed = CurrentProjectile->MovementComponent->InitialSpeed;

	
	float HorizontalDistance = 0;
	FVector NewLaunchPoint;

	HorizontalDistance = FVector2D(FVector2D(TargetPoint.X, TargetPoint.Y) - FVector2D(SockLoc.X, SockLoc.Y)).Size();

	float Height = TargetPoint.Z - SockLoc.Z;

	float Gravity = 980;
	float calculation = (Speed * Speed * Speed * Speed) - (Gravity * (Gravity * (HorizontalDistance * HorizontalDistance) + (2 * Height * (Speed * Speed))));
	if (calculation <= 0)
		return 0;
	else
	{
		
		float NewCalculation = FMath::Sqrt(calculation);
		float Result = 0;
		if(bArcFire)
		{ 
			 Result = UKismetMathLibrary::DegAtan(((Speed * Speed) + NewCalculation) / (Gravity * HorizontalDistance));
		}
		else
		{
			Result = UKismetMathLibrary::DegAtan(((Speed * Speed) - NewCalculation) / (Gravity * HorizontalDistance));
		}
		return Result;
	}

	return 0;
}

void AWeapon::ServerSetTargetLocation_Implementation(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
}
bool AWeapon::ServerSetTargetLocation_Validate(FVector NewTargetLocation)
{
	return true;
}

void AWeapon::DrawProjectilePath()
{
	
}