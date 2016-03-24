// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, Category = Ammo)	int32 MaxAmmo;
	UPROPERTY(EditDefaultsOnly, Category = Ammo)	int32 ShotCost;
	UPROPERTY(EditDefaultsOnly, Category = Config)	float TimeBetweenShots;
	UPROPERTY(EditDefaultsOnly, Category = Config)	float WeaponRange;
	UPROPERTY(EditDefaultsOnly, Category = Config)	float WeaponSpread;
};

UCLASS()
class GOLM_API AWeapon : public AActor
{
	GENERATED_BODY()


	

	UFUNCTION(Client, Unreliable, NetMulticast)
		void ClientPlayLaunchEffects();
		void ClientPlayLaunchEffects_Implementation();
	
public:	

	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	bool bProjectileCollisionPossible;

	UPROPERTY(EditDefaultsOnly, Category = Config)						FWeaponData WeaponConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)	UBoxComponent *CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	USkeletalMeshComponent *WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	UParticleSystemComponent *WeaponMuzzle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config, Replicated)	AProjectile *CurrentProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CompatibleProjectiles)
		TArray<TSubclassOf<AProjectile>> CompatibleProjectiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	UAudioComponent *WeaponAudio;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)	USoundBase *WeaponAutoMaticEnd;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	bool bAutomatic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	float FiringRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	FName Name;

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)		
		void WeaponFire(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void PlayLaunchEffects();
	
	//***Deletion is set to true when the Owner gets deleted so the weapon can be deleted with it
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void Death();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void SetRagDoll(bool value);

	void LaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(BlueprintCallable, Category = Changes)
		void SetNewProjectile(AProjectile *NewProjectile);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetNewProjectile(AProjectile *NewProjectile);
		void ServerSetNewProjectile_Implementation(AProjectile *NewProjectile);
		bool ServerSetNewProjectile_Validate(AProjectile *NewProjectile);



	void ToggleProjectileCollision(bool CollisionPossible);


protected:

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerLaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation);
		void ServerLaunchProjectile_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);
		bool ServerLaunchProjectile_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);




};
