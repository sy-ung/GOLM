// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Weapon.h"
#include "GOLMPlayerState.h"
#include "GameFramework/Character.h"
#include "GOLMCharacter.generated.h"



UCLASS(Blueprintable)
class AGOLMCharacter : public ACharacter
{
	GENERATED_BODY()


	AGOLMPlayerState *PlayerState;

	FVector Direction;

	void UpdateAim();
	UFUNCTION(Server, Reliable, WithValidation)
		void UpdateAim_ServerUpdate(float NewWeaponAimPitch, float NewActorYaw);
		void UpdateAim_ServerUpdate_Implementation(float NewWeaponAimPitch, float NewActorYaw);
		bool UpdateAim_ServerUpdate_Validate(float NewWeaponAimPitch, float NewActorYaw);


	void TracePath(FVector &start, FVector &finish);


	
	FRotator WeaponMuzzleRotation;
	FVector WeaponMuzzleLocation;



	bool bAimPitchable;

	float DefaultNetCullDistanceSquared;

public:
	AGOLMCharacter();


	virtual void PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker);
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	UCameraComponent *PlayerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	USpringArmComponent* PlayerCameraBoom;


	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Scale")	float WeaponScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Available Weapons")	TSubclassOf<class AWeapon> RocketLauncher;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Available Weapons")	TSubclassOf<class AWeapon> Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyMovements)			float MovingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyMovements)			float TurnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyMovements)			float BoostSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraData)			float CameraRotationSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraData)			float CameraMovementSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraData)			float MaxCameraHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraData)			float MinCameraHeight;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = PlayerData)		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerData)		float deathTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerData)		float RespawnTimeCheck;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)		bool bBoosting;
	UPROPERTY(Replicated)		bool bAlive;
	UPROPERTY(Replicated)		bool bAbleToShoot;
	UPROPERTY(Replicated)		float TimeUntilRespawn;

	bool bStartShooting;
	float TimeBeforeNextShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MyActions)		bool bRotatingCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MyActions)		bool bMovingCamera;

	UPROPERTY(BlueprintReadOnly, Replicated)		float RelativeForward;
	UPROPERTY(BlueprintReadOnly, Replicated)		float RelativeRight;


	UPROPERTY(BlueprintReadOnly, Replicated)		float WeaponAimPitch;
	UPROPERTY(BlueprintReadOnly, Replicated)		FName CurrentLevelStream;
	UPROPERTY(BlueprintReadOnly, Replicated)		AWeapon *CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)					FName OriginalCollisionProfile;
	UPROPERTY(BlueprintReadOnly)					FName NoPawnCollisionProfile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	FRotator FinalOrientation;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMoving;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bHasHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingUp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingRight;
	
	
	UFUNCTION()
		virtual void BeginPlay() override;


	UFUNCTION()
		virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
		virtual void TornOff() override;

	UFUNCTION()
		virtual void Destroyed() override;

	UFUNCTION()
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
		);

	//***Is also called on log out to remove weapon assets
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void Death();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void Respawn();

	UFUNCTION(Client, Reliable, WithValidation)
		void ClientRespawn();
		void ClientRespawn_Implementation();
		bool ClientRespawn_Validate();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void GotoLockerRoom();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRespawn();
		void ServerRespawn_Implementation();
		bool ServerRespawn_Validate();

	UFUNCTION(Client, Reliable, WithValidation,NetMulticast)
		void ClientDeath();
		void ClientDeath_Implementation();
		bool ClientDeath_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerDeath();
		void ServerDeath_Implementation();
		bool ServerDeath_Validate();
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void CalculateRelativeDirectionScale();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUpdateRelativeDirectionScale(float relativeforwardscale, float relativerightscale);
		void ServerUpdateRelativeDirectionScale_Implementation(float relativeforwardscale, float relativerightscale);
		bool ServerUpdateRelativeDirectionScale_Validate(float relativeforwardscale, float relativerightscale);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void ZoomCamera(float deltaZoom);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void FireWeapon();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFireWeapon();
		void ServerFireWeapon_Implementation();
		bool ServerFireWeapon_Validate();

	UFUNCTION(BlueprintCallable, Category = StuffIMustDo)
		void Equip(EGetWeapon NewWeapon, EEquipSlot In);

		void GetEquippedWeapons();
	UFUNCTION(Client, Reliable)
		void ClientGetEquippedWeapons();
		void ClientGetEquippedWeapons_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquip(EGetWeapon NewWeapon, EEquipSlot In);
		void ServerEquip_Implementation(EGetWeapon NewWeapon, EEquipSlot In);
		bool ServerEquip_Validate(EGetWeapon NewWeapon, EEquipSlot In);

	//*****-1 to move backwards, 1 to move forwards, 0 to not move
	UFUNCTION(BlueprintCallable, Category = MyMovements)
		void MoveCheck();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMove(FRotator direction,bool status);
		void ServerMove_Implementation(FRotator direction,bool status);
		bool ServerMove_Validate(FRotator direction,bool status);

	UFUNCTION(BlueprintCallable, Category = MyMovments)
		void Boost();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBoost(FVector LaunchDirection);
		void ServerBoost_Implementation(FVector LaunchDirection);
		bool ServerBoost_Validate(FVector LaunchDirection);


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void RotateCamera();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MoveCamera();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		AWeapon *GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		float GetPlayerSpeed();

	UFUNCTION(BlueprintCallable, Category = PlayerData)
		FName GetCurrentLevelStream();

		void LoadEntranceLevel(FName EntranceLevelName);
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientLoadEntranceLevel(FName EntranceLevelName);
		void ClientLoadEntranceLevel_Implementation(FName EntranceLevelName);
		bool ClientLoadEntranceLevel_Validate(FName EntranceLevelName);


		void MoveToEntrance(FName EntranceLevelName);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMoveToEntrance(FName EntranceLevelName);
		void ServerMoveToEntrance_Implementation(FName EntranceLevelName);
		bool ServerMoveToEntrance_Validate(FName EntranceLevelName);

		void InitLevels();
	UFUNCTION(Client,Reliable,WithValidation)
		void ClientInitLevels();
		void ClientInitLevels_Implementation();
		bool ClientInitLevels_Validate();

		void SetRagDoll(bool value);
	UFUNCTION(Client, Reliable, NetMulticast)
		void ClientSetRagDoll(bool value);
		void ClientSetRagDoll_Implementation(bool value);

	UFUNCTION(BlueprintCallable, Category = PlayerCharacterAction)
		void RecieveDamage(float damage);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRecieveDamage(float damage);
		void ServerRecieveDamage_Implementation(float damage);
		bool ServerRecieveDamage_Validate(float damage);


		void TurnOnNoCollisionProfile(bool ToggleOn);
	UFUNCTION(Client, Reliable, NetMultiCast)
		void ClientTurnOnNoCollisionProfile(bool value);
		void ClientTurnOnNoCollisionProfile_Implementation(bool value);
			
};

