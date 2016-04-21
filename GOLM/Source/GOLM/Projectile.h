// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.h"
#include "Projectile.generated.h"

UCLASS(Blueprintable)
class GOLM_API AProjectile : public AActor
{
	GENERATED_BODY()

	FName NoPawnCollisionProfile;

public:
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	void BeginParticle();
	UFUNCTION(Client, Unreliable, NetMulticast)
		void ClientBeginParticle();
		void ClientBeginParticle_Implementation();

	virtual void NotifyHit
		(
			class UPrimitiveComponent * MyComp,
			AActor * Other,
			class UPrimitiveComponent * OtherComp,
			bool bSelfMoved,
			FVector HitLocation,
			FVector HitNormal,
			FVector NormalImpulse,
			const FHitResult & Hit
		) 
	override;




	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UBoxComponent *CollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjecitleSetyp)		USphereComponent *ExplosiveSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UStaticMeshComponent *ProjectileMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UParticleSystemComponent *MuzzleParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UParticleSystemComponent *RearParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UParticleSystemComponent *DeathParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		URadialForceComponent *ProjectileForceComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileSetup)		bool bExplosive;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileSetup)		float  ExplosionRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileSetup)		FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileSetup)		UTexture2D *Image;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileMovement)	class UProjectileMovementComponent *MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		float LifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		bool bVTOL;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		float VTOLSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		float VTOLHeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		float VTOLDropRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup)		float VTOLStartTimer;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Explosion)
		TSubclassOf<AExplosion> ExplosionBP;

	bool bBeginVTOL;
	bool bVTOLStage1Complete;
	bool bVTOLStage2Complete;
	bool bVTOLStage3Complete;
	UPROPERTY(Replicated) FVector TargetLocation;
	FVector VTOLStartLocation;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UAudioComponent *MoveSounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ProjectileSetup)		UAudioComponent *DeathSounds;
	
	//Only Server needs to know about Current Level Stream
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectileData)		FName CurrentLevelStream;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileCluster) TSubclassOf<AProjectile> ClusterProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileCluster) float ClusterProjectileScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectileCluster) float ClusterLaunchDistance;
	//Set this to false if this is a Cluster Projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup) bool bIsClusterProjectile;
	//The Spread in degress
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup) float ClusterSpreadAmount;
	//The number of cluster items
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileSetup) float NumberInCluster;
	void FireCluster();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void OnDeath();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void InflictExplosiveDamage();

	void ToggleNoPawnCollision();
	void SetHomingPosition(FVector Location);

	UFUNCTION()
		virtual void Destroyed() override;

private:

	UPROPERTY(Replicated)	
	bool Alive;

	FVector Direction;
	void VTOLMovement(float DeltaSeconds);

};
