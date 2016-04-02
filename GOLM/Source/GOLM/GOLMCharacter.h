// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Weapon.h"
#include "GOLMPlayerState.h"
#include "GOLMPlayerStart.h"
#include "GameFramework/Character.h"
#include "GOLMCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerCollisionProfile : uint8
{
	LOCKER			UMETA(DisplayName = "Locker Room Profile Collision"),
	DEATH			UMETA(DisplayName = "Death profile collision"),
	REGULAR			UMETA(DisplayName = "Collision As Pawn")

};


UCLASS(Blueprintable)
class AGOLMCharacter : public ACharacter
{
	GENERATED_BODY()


	AGOLMPlayerState *PlayerState;

	FVector Direction;




	void TracePath(FVector &start, FVector &finish);


	
	FRotator WeaponMuzzleRotation;
	FVector WeaponMuzzleLocation;

	class AGOLMLevelStreamBeacon *LockerRoomBeaconReference;


	bool bAimPitchable;


	float DefaultNetCullDistanceSquared;

public:
	AGOLMCharacter();

	bool bIsInMenu;

	virtual void PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker);
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	ACameraActor *PlayerCameraActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	ACameraActor *FrontCameraActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	ACameraActor *LeftShoulderCameraActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	ACameraActor *RightShoulderCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	UCameraComponent *PlayerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	USpringArmComponent* PlayerCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)	UCameraComponent *EquipmentCameraFront;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	USpringArmComponent* EquipmentCameraFrontBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)	UCameraComponent *EquipmentCameraLeftShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	USpringArmComponent* EquipmentCameraLeftShoulderBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)	UCameraComponent *EquipmentCameraRightShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)	USpringArmComponent* EquipmentCameraRightShoulderBoom;


	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Scale")	float WeaponScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Available Weapons")	TSubclassOf<class AWeapon> RocketLauncher;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Available Weapons")	TSubclassOf<class AWeapon> Rifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MiniMapCamera")		TSubclassOf<class AGOLMMiniMapCamera> MiniMapCamera;
	class AGOLMMiniMapCamera *MiniMapCameraReference;

	void ZoomMiniMapCamera(float value);

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
	UPROPERTY(Replicated)		float TimeUntilRespawn;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MyActions)		bool bRotatingCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MyActions)		bool bMovingCamera;

	UPROPERTY(BlueprintReadOnly, Replicated)		float RelativeForward;
	UPROPERTY(BlueprintReadOnly, Replicated)		float RelativeRight;


	UPROPERTY(BlueprintReadOnly, Replicated)		float WeaponAimPitch;
	UPROPERTY(BlueprintReadOnly, Replicated)		FName CurrentLevelStream;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShowCompatibleProjectiles)		AWeapon *CurrentHandWeapon;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShowCompatibleProjectiles)		AWeapon *CurrentLeftShoulderWeapon;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShowCompatibleProjectiles)		AWeapon *CurrentRightShoulderWeapon;

	UPROPERTY(BlueprintReadOnly)					FName OriginalCollisionProfile;
	UPROPERTY(BlueprintReadOnly)					FName NoPawnCollisionProfile;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")				FVector HandSupportLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")				FVector GripBoneLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")				FVector LeftPalmLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CharacterData")	FRotator FinalOrientation;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMoving;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bHasHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingUp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData")	bool bMovingRight;

		void UpdateAim(FVector MouseHit);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUpdateAim(float NewPitch, float NewYaw);
		void ServerUpdateAim_Implementation(float NewPitch, float NewYaw);
		bool ServerUpdateAim_Validate(float NewPitch, float NewYaw);

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

	UFUNCTION(Client, Reliable, WithValidation)
		void ClientDeath();
		void ClientDeath_Implementation();
		bool ClientDeath_Validate();
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void CalculateRelativeDirectionScale();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUpdateRelativeDirectionScale(float relativeforwardscale, float relativerightscale);
		void ServerUpdateRelativeDirectionScale_Implementation(float relativeforwardscale, float relativerightscale);
		bool ServerUpdateRelativeDirectionScale_Validate(float relativeforwardscale, float relativerightscale);


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void FireHandWeapon(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void FireLeftShoulderWeapon(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void FireRightShoulderWeapon(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffIMustDo)
		void Equip(AWeapon *NewWeapon, EEquipSlot In);

		void GetEquippedWeapons();
	UFUNCTION(Client, Reliable)
		void ClientGetEquippedWeapons();
		void ClientGetEquippedWeapons_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquip(AWeapon *NewWeapon, EEquipSlot In);
		void ServerEquip_Implementation(AWeapon *NewWeapon, EEquipSlot In);
		bool ServerEquip_Validate(AWeapon *NewWeapon, EEquipSlot In);

	//*****-1 to move backwards, 1 to move forwards, 0 to not move
	UFUNCTION(BlueprintCallable, Category = MyMovements)
		void MoveCheck();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMove(FRotator direction);
		void ServerMove_Implementation(FRotator direction);
		bool ServerMove_Validate(FRotator direction);




	UFUNCTION(BlueprintCallable, Category = MyMovments)
		void Boost();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBoost(FVector LaunchDirection);
		void ServerBoost_Implementation(FVector LaunchDirection);
		bool ServerBoost_Validate(FVector LaunchDirection);


		void BoostCharacter(FVector LaunchVelocity);
	UFUNCTION(Client, Reliable, NetMulticast)
		void ClientBoostCharacter(FVector NewLaunchVelocity);
		void ClientBoostCharacter_Implementation(FVector NewLaunchVelocity);


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void RotateCamera();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MoveCamera();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void ZoomCamera(float DeltaHeight);
		float NewCameraHeight;


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		AWeapon *GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		float GetPlayerSpeed();

	UFUNCTION(BlueprintCallable, Category = PlayerData)
		FName GetCurrentLevelStream();

		void LoadEntranceLevel(class AGOLMLevelStreamBeacon *LevelBeacon);
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientLoadEntranceLevel(class AGOLMLevelStreamBeacon *LevelBeacon);
		void ClientLoadEntranceLevel_Implementation(class AGOLMLevelStreamBeacon *LevelBeacon);
		bool ClientLoadEntranceLevel_Validate(class AGOLMLevelStreamBeacon *LevelBeacon);

		void LoadEntranceLevel(FName EntranceLevelName);
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientLoadEntranceLevelNameOnly(FName EntranceLevelName);
		void ClientLoadEntranceLevel_ImplementationNameOnly(FName EntranceLevelNamen);
		bool ClientLoadEntranceLevel_ValidateNameOnly(FName EntranceLevelName);


		
		void MoveToEntrance(FName EntranceLevelNameTag);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMoveToEntranceTagOnly(FName EntranceLevelNameTag);
		void ServerMoveToEntranceTagOnly_Implementation(FName EntranceLevelNameTag);
		bool ServerMoveToEntranceTagOnly_Validate(FName EntranceLevelNameTag);

		void MoveToEntrance(class AGOLMLevelStreamBeacon *LevelBeacon);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerMoveToEntrance(class AGOLMLevelStreamBeacon *LevelBeacon);
		void ServerMoveToEntrance_Implementation(class AGOLMLevelStreamBeacon *LevelBeacon);
		bool ServerMoveToEntrance_Validate(class AGOLMLevelStreamBeacon *LevelBeacon);

		void Init();
	UFUNCTION(Client,Reliable,WithValidation)
		void ClientInit();
		void ClientInit_Implementation();
		bool ClientInit_Validate();

		void SetRagDoll(bool value);
	UFUNCTION(Client, Reliable,NetMulticast)
		void ClientSetRagDoll(bool value);
		void ClientSetRagDoll_Implementation(bool value);

	UFUNCTION(BlueprintCallable, Category = PlayerCharacterAction)
		void RecieveDamage(float damage);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRecieveDamage(float damage);
		void ServerRecieveDamage_Implementation(float damage);
		bool ServerRecieveDamage_Validate(float damage);

		void SetPawnCollisionType(EPlayerCollisionProfile NewCollision);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPawnCollisionType(EPlayerCollisionProfile NewCollisionType);
		void ServerSetPawnCollisionType_Implementation(EPlayerCollisionProfile NewCollisionType);
		bool ServerSetPawnCollisionType_Validate(EPlayerCollisionProfile NewCollisionType);
	UFUNCTION(Client,Reliable, NetMulticast)
		void ClientSetPawnCollisionType(EPlayerCollisionProfile NewCollisionType);
		void ClientSetPawnCollisionType_Implementation(EPlayerCollisionProfile NewCollisionType);

	UFUNCTION()
		void OnRep_ShowCompatibleProjectiles();

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;
	
};

