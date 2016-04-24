// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMGameState.h"
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
	
	WeaponAutoMaticEnd = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponAutomaticEnd"));
	WeaponAutoMaticEnd->AttachTo(RootComponent);
	bNetUseOwnerRelevancy = true;
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
		if (Cast<AGOLMCharacter>(GetOwner())->bIsAI)
		{
			CurrentProjectile = CompatibleProjectiles[FMath::RandRange(0, CompatibleProjectiles.Num() - 1)].GetDefaultObject();
		}

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
		if (TotalAmmoCount <= 0)
		{
			TotalAmmoCount = 0;
			if (bReloading)
				bReloading = false;
		}
		if (bReloading)
		{
			if (CurrentAmmoCount == MagazineSize)
				bReloading = false;
		}

		if ((TotalAmmoCount + CurrentAmmoCount) <= 0)
		{
			bStartShooting = false;
			bAbleToShoot = false;
		}
		else
		{
			if (CurrentAmmoCount > 0)
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
			else
			{
				bStartShooting = false;
				bReloading = true;
			}

			if(TotalAmmoCount>0)
			{
				if (bReloading)
				{
					bStartShooting = false;
					bAbleToShoot = false;

					if (ReloadTimer >= ReloadTimeFinish)
					{
						bReloading = false;
						ReloadTimer = 0;
						int32 DeltaAmmo = MagazineSize - CurrentAmmoCount;

						if (DeltaAmmo <= TotalAmmoCount)
						{
							TotalAmmoCount -= DeltaAmmo;
							CurrentAmmoCount += DeltaAmmo;
						}
						else
						{
							CurrentAmmoCount += TotalAmmoCount;
							TotalAmmoCount = 0;
						}

						TimeBeforeNextShot = 0;
						bAbleToShoot = true;
					}
					else
					{
						ReloadTimer += DeltaSeconds;
					}
				}
			}
		}
	}

	if (Cast<AGOLMGameState>(UGameplayStatics::GetGameState(GetWorld()))->IsGameOver)
	{
		bStartShooting = false;
	}

	if(GetOwner() != NULL)
	{
		if (Cast<AGOLMCharacter>(GetOwner())->bAlive)
		{
			if (bStartShooting)
			{
				if (bAbleToShoot)
				{
					if (CurrentAmmoCount > 0)
					{
						FRotator WeaponMuzzleRotation = WeaponMesh->GetSocketRotation("MuzzleFlash");
						FVector WeaponMuzzleLocation = WeaponMesh->GetSocketLocation("MuzzleFlash");
						FireWeapon(WeaponMuzzleLocation, WeaponMuzzleRotation);
					}
					else
						bStartShooting = false;
				}
			}
		}
		else
			bStartShooting = false;
	}



	if(!bStartShooting)
	{
		if (bAutomatic)
		{
			if (WeaponAudio->Sound != NULL)
				if (WeaponAudio->IsPlaying())
				{
					WeaponAudio->Stop();
					if (WeaponAutoMaticEnd->Sound != NULL)
						WeaponAutoMaticEnd->Play();
				}


		}
	}
	

	
	
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, CurrentProjectile);
	DOREPLIFETIME(AWeapon, bAbleToShoot);
	DOREPLIFETIME(AWeapon, bStartShooting);
	DOREPLIFETIME(AWeapon, CurrentAmmoCount);
	DOREPLIFETIME(AWeapon, TotalAmmoCount);

	DOREPLIFETIME(AWeapon, bReloading);
	DOREPLIFETIME(AWeapon, ReloadTimer);
}




void AWeapon::FireWeapon(FVector StartLocation, FRotator StartRotation)
{
	if (!bAutomatic)
	{
		bStartShooting = false;
	}
	bAbleToShoot = false;

	if(Role != ROLE_Authority)
		ServerFireWeapon(StartLocation,StartRotation);
		
	
	if (Role == ROLE_Authority)
	{
		bAbleToShoot = false;
		TimeBeforeNextShot = FiringRate;

		LaunchProjectile(StartLocation, StartRotation);
	}
	
}
void AWeapon::ServerFireWeapon_Implementation(FVector StartLocation, FRotator StartRotation)
{
	FireWeapon(StartLocation,StartRotation);
}
bool AWeapon::ServerFireWeapon_Validate(FVector StartLocation, FRotator StartRotation)
{
	return true;
}





void AWeapon::LaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	PlayLaunchEffects();

	if(Role == ROLE_Authority)
	{

		AGOLMCharacter *WeaponOwner = Cast<AGOLMCharacter>(GetOwner());
		if (UKismetSystemLibrary::IsValid(WeaponOwner))
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = WeaponOwner;
			spawnParams.Instigator = WeaponOwner;

			if (bIsSpread)
			{
				LaunchSpreadProjectile(MuzzleLocation, MuzzleRotation);
			}
			else
			{
				AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(CurrentProjectile->GetClass(), MuzzleLocation, MuzzleRotation, spawnParams);

				projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetOwner())->CurrentLevelStream;
				projectile->SetActorScale3D(this->GetActorScale3D());
				projectile->TargetLocation = WeaponOwner->TargetLocation;

			}
			CurrentAmmoCount--;
			
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
	AGOLMCharacter *WeaponOwner = Cast<AGOLMCharacter>(GetOwner());
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = WeaponOwner;
	spawnParams.Instigator = WeaponOwner;
	for (int32 i = 0; i < NumberOfSpread;i++)
	{
		
		int32 RandomSeed = FMath::Rand();
		FRandomStream SpreadRandom(RandomSeed);
		float SpreadCone = FMath::DegreesToRadians(SpreadAmount * 0.5);
		FVector Direction = SpreadRandom.VRandCone(MuzzleRotation.Vector(), SpreadCone, SpreadCone);
		spawnParams.Instigator = Cast<AGOLMCharacter>(GetOwner());
		AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(CurrentProjectile->GetClass(), MuzzleLocation, Direction.Rotation(), spawnParams);
		projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetOwner())->CurrentLevelStream;
		projectile->SetActorScale3D(this->GetActorScale3D());

	}
	

}

void AWeapon::PlayLaunchEffects()
{
	FVector WMFL = WeaponMesh->GetSocketLocation("MuzzleFlash");
	FRotator WMFR = WeaponMesh->GetSocketRotation("MuzzleFlash");

	if(CurrentProjectile->MuzzleParticle->Template != NULL)
	{
		//UGameplayStatics::SpawnEmitterAttached(CurrentProjectile->MuzzleParticle->Template, WeaponMesh, NAME_None, WMFL, WMFR,EAttachLocation::SnapToTarget);
		UGameplayStatics::SpawnEmitterAtLocation(WeaponMesh, CurrentProjectile->MuzzleParticle->Template, WMFL, WMFR);
	}
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
	
	if (CurrentProjectile->bVTOL)
		return 20.0f;

	FVector SockLoc = WeaponMesh->GetSocketLocation("MuzzleFlash");
	

	float Speed = 0;

	Speed = CurrentProjectile->MovementComponent->InitialSpeed;

	
	float HorizontalDistance = 0;
	FVector NewLaunchPoint;

	HorizontalDistance = FVector2D(FVector2D(TargetPoint.X, TargetPoint.Y) - FVector2D(SockLoc.X, SockLoc.Y)).Size();

	float Height = TargetPoint.Z - SockLoc.Z;

	float Gravity = 980 * CurrentProjectile->MovementComponent->ProjectileGravityScale;
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

void AWeapon::DrawProjectilePath()
{
	
}

void AWeapon::Reload()
{
	if (Role != ROLE_Authority)
		ServerReload();
	else
		bReloading = true;
}
void AWeapon::ServerReload_Implementation()
{
	Reload();
}
bool AWeapon::ServerReload_Validate()
{
	return true;
}