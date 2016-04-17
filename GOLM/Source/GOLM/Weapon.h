// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Projectile.h"
#include "GOLMPlayerState.h"
#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	EEquipSlot CompatibleWeaponSlotType;
	EEquipSlot CurrentSlotType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)	UBoxComponent *CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	UParticleSystemComponent *WeaponProjectileTraceImpact;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config, Replicated)	AProjectile *CurrentProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CompatibleProjectiles)
		TArray<TSubclassOf<AProjectile>> CompatibleProjectiles;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	UAudioComponent *WeaponAudio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)	UAudioComponent *WeaponAutoMaticEnd;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	bool bAutomatic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	float FiringRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)	bool bArcFire;

	UPROPERTY(Replicated) bool bAbleToShoot;
	UPROPERTY(Replicated) bool bStartShooting;
	float TimeBeforeNextShot;

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void FireWeapon(FVector StartLocation, FRotator StartRotation);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFireWeapon(FVector StartLocation, FRotator StartRotation);
		void ServerFireWeapon_Implementation(FVector StartLocation, FRotator StartRotation);
		bool ServerFireWeapon_Validate(FVector StartLocation, FRotator StartRotation);



	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void PlayLaunchEffects();
	
	//***Deletion is set to true when the Owner gets deleted so the weapon can be deleted with it
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void Death();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void SetRagDoll(bool value);

	void LaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config) bool bIsSpread;
	//The Length of spread in degrees
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config) float SpreadAmount;
	//Number of projectile for each shot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config) float NumberOfSpread;
	void LaunchSpreadProjectile(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(BlueprintCallable, Category = Changes)
		void SetNewProjectile(AProjectile *NewProjectile);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetNewProjectile(AProjectile *NewProjectile);
		void ServerSetNewProjectile_Implementation(AProjectile *NewProjectile);
		bool ServerSetNewProjectile_Validate(AProjectile *NewProjectile);

		float CalculateProjectilePath(FVector TargetPoint);
		void DrawProjectilePath();

		void ToggleProjectileCollision(bool CollisionPossible);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config) UTexture2D *WeaponImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo) int32 MagazineSize;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Ammo) int32 CurrentAmmoCount;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Ammo) int32 TotalAmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo) int32 MaxAmmo;

	UPROPERTY(Replicated) bool bReloading;
	UPROPERTY(Replicated) float ReloadTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload) float ReloadTimeFinish;
	

		void Reload();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReload();
		void ServerReload_Implementation();
		bool ServerReload_Validate();

protected:

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerLaunchProjectile(FVector MuzzleLocation, FRotator MuzzleRotation);
		void ServerLaunchProjectile_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);
		bool ServerLaunchProjectile_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);




};
