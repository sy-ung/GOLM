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

	WeaponMuzzle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponMuzzle"));
	WeaponMuzzle->AttachTo(RootComponent);
	
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
	if(Role == ROLE_Authority)
		CurrentProjectile = CompatibleProjectiles[0].GetDefaultObject();
}

// Called every frame
void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bAutomatic)
	{
		if(WeaponAudio->IsPlaying())
		{
			if (!Cast<AGOLMCharacter>(GetOwner())->bStartShooting)
			{
				WeaponAudio->Stop();
				UGameplayStatics::SpawnSoundAtLocation(WeaponMesh, WeaponAutoMaticEnd, WeaponMesh->GetSocketLocation("MuzzleFlash"));
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Cyan, FString::FromInt(Cast<AGOLMCharacter>(GetOwner())->bStartShooting));
	}
	

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, CurrentProjectile);
}

void AWeapon::WeaponFire(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	LaunchProjectile(MuzzleLocation,MuzzleRotation);

}
void AWeapon::LaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	PlayLaunchEffects();
	if(Role == ROLE_Authority)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Instigator = Cast<AGOLMCharacter>(GetOwner());
		
		AProjectile *projectile = GetWorld()->SpawnActor<AProjectile>(CurrentProjectile->GetClass(),MuzzleLocation,MuzzleRotation,spawnParams);

		projectile->CurrentLevelStream = Cast<AGOLMCharacter>(GetOwner())->CurrentLevelStream;
		projectile->SetActorScale3D(this->GetActorScale3D());
		//Instigator->MoveIgnoreActorAdd(projectile);	
		//Instigator->MoveIgnoreActorAdd(GetOwner());
		
	}

	if (Role != ROLE_Authority)
	{
		ServerLaunchProjectile(MuzzleLocation, MuzzleRotation);
	}
		
}

void AWeapon::ServerLaunchProjectile_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	WeaponFire(MuzzleLocation, MuzzleRotation);
}

bool AWeapon::ServerLaunchProjectile_Validate(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	return true;
}

void AWeapon::PlayLaunchEffects()
{
	FVector WMFL = WeaponMesh->GetSocketLocation("MuzzleFlash");
	FRotator WMFR = WeaponMesh->GetSocketRotation("MuzzleFlash");

	if(WeaponMuzzle != NULL)
		UGameplayStatics::SpawnEmitterAtLocation(WeaponMesh, WeaponMuzzle->Template, WMFL, WMFR);

	if(WeaponAudio->Sound != NULL)
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