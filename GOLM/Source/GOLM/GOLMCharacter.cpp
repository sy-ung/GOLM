// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMGameMode.h"
#include "GOLMPlayerController.h"
#include "Kismet/KismetMathLibrary.h"



AGOLMCharacter::AGOLMCharacter()
{
	// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	PlayerCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerCameraBoom->AttachTo(RootComponent);
	PlayerCameraBoom->RelativeRotation = FRotator(-60.0f, 0.0f, 0.0f);
	PlayerCameraBoom->bDoCollisionTest = false;
	

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->AttachTo(PlayerCameraBoom, USpringArmComponent::SocketName);

	PrimaryActorTick.bCanEverTick = true;
	
	Health = 100.0f;
	bAlive = true;
	
	SetReplicates(true);


	//MovementComponent = CreateDefaultSubobject<UGOLMPawnMovementComponent>(TEXT("GOLM Movement Component"));
	//MovementComponent->UpdatedComponent = RootComponent;
}

void AGOLMCharacter::InitLevels()
{
	if (Role == ROLE_Authority)
		ClientInitLevels();

	if (IsLocallyControlled() && Role != ROLE_Authority)
	{
		UWorld *World = GetWorld();
		UGameplayStatics::GetStreamingLevel(World, "LockerRoom")->bShouldBeVisible = false;
		UGameplayStatics::GetStreamingLevel(World, "GameLevel")->bShouldBeVisible = false;
		UGameplayStatics::GetStreamingLevel(World, "Bottomworld")->bShouldBeVisible = false;

		//TArray<AActor*, FDefaultAllocator> SpawnLocs;
		//UGameplayStatics::GetAllActorsOfClass(UGameplayStatics::GetStreamingLevel(World, "Bottomworld")->GetWorld(), APlayerStart::StaticClass(), SpawnLocs);

		//for (int32 i = 0; i < SpawnLocs.Num(); i++)
		//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Cyan, SpawnLocs[i]->GetActorLocation().ToString());
	}

}

void AGOLMCharacter::ClientInitLevels_Implementation()
{
	if (Role != ROLE_Authority)
		InitLevels();
}
bool AGOLMCharacter::ClientInitLevels_Validate()
{
	return true;
}

void AGOLMCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = TurnSpeed;
	PlayerCameraBoom->TargetArmLength = MinCameraHeight;

	//PlayerState = Cast<AGOLMPlayerState>(Cast<AGOLMPlayerController>(GetController())->PlayerState);
	
	RespawnTimeCheck = 0;

	//Equip(EGetWeapon::ROCKET_LAUNCHER, EEquipSlot::HAND_SLOT);
	DefaultNetCullDistanceSquared = NetCullDistanceSquared;
	bAlive = true;
	Health = 100;
	deathTimer = 5.0f;

	OriginalCollisionProfile = GetCapsuleComponent()->GetCollisionProfileName();
	NoPawnCollisionProfile = "NoPawn";
	GetEquippedWeapons();
}

void AGOLMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AGOLMCharacter, WeaponAim);
	DOREPLIFETIME(AGOLMCharacter, FinalOrientation);
	DOREPLIFETIME(AGOLMCharacter, bMoving);
	DOREPLIFETIME(AGOLMCharacter, CurrentWeapon);
	DOREPLIFETIME(AGOLMCharacter, CurrentLevelStream);
	DOREPLIFETIME(AGOLMCharacter, bBoosting);
	DOREPLIFETIME(AGOLMCharacter, bAbleToShoot);
	DOREPLIFETIME(AGOLMCharacter, Health);
	DOREPLIFETIME(AGOLMCharacter, bAlive);
	DOREPLIFETIME(AGOLMCharacter, TimeUntilRespawn);

}

void AGOLMCharacter::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

}

//***RealViewer refers to player controller and ViewTarget refers to Character
bool AGOLMCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	const AGOLMCharacter *TargetCharacter = Cast<AGOLMCharacter>(ViewTarget);

	if (TargetCharacter == this)
		return true;
	if (TargetCharacter->CurrentLevelStream == "LockerRoom")
		return false;
	if (TargetCharacter->CurrentLevelStream == CurrentLevelStream)
		if (TargetCharacter->bAlive)
			return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	return false;


	if (TargetCharacter->CurrentLevelStream == "LockerRoom")
	{
		if (TargetCharacter == this)
			return true;
		else
		{
			return false;
		}
	}
	else if (TargetCharacter->CurrentLevelStream == CurrentLevelStream)
	{
		
		//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Green, "IS RELEVENT");
		if(TargetCharacter->bAlive)
			return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
		else
		{
			if (TargetCharacter == this)
				return true;
			else
				return false;
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "IS NOT RELEVENT");
		return false;
	}
}


void AGOLMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Role == ROLE_Authority)
	{
		if (bAlive)
		{
			if (Health <= 0)
			{
				Death();
			}
			if (CurrentWeapon != NULL)
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
		}
		else
		{
			RespawnTimeCheck += DeltaSeconds;
			TimeUntilRespawn = deathTimer - RespawnTimeCheck;
			if (RespawnTimeCheck > deathTimer)
			{
				RespawnTimeCheck = 0;
				Respawn();
				SetRagDoll(false);
			}
		}
	}
	if(Role != ROLE_Authority || IsLocallyControlled())
	{
		//GEngine->ClearOnScreenDebugMessages();
		//if (bAlive)
		//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Green, "Health: " + FString::SanitizeFloat(Health));
		//else
		//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Respawn in: " + FString::SanitizeFloat(TimeUntilRespawn));

		if (bStartShooting)
		{
			if (bAbleToShoot)
			{
				FireWeapon();
			}
		}
		if (bRotatingCamera)
			RotateCamera();

		if (bMovingCamera)
			MoveCamera();

		bAimPitchable = GetMovementComponent()->IsFalling();
				

		MoveCheck();
		UpdateAim();
	}
	//SetActorRelativeRotation(FinalOrientation);

	//Everything here is ticked on the server as well as the client
	//Variables must be changed from client to server to see the effect
	if(bAlive)
	{
		if(bMoving)
		{
			SetActorRotation(FMath::Lerp(GetActorForwardVector().Rotation(), FinalOrientation, 0.25f));
			GetCharacterMovement()->AddInputVector(GetActorForwardVector() * (MovingSpeed)* DeltaSeconds);
		}
		Boost(DeltaSeconds, bBoosting);
	}
}

void AGOLMCharacter::NotifyHit
(class UPrimitiveComponent * MyComp,	AActor * Other,class UPrimitiveComponent * OtherComp,	bool bSelfMoved,
	FVector HitLocation,	FVector HitNormal,	FVector NormalImpulse,	const FHitResult & Hit
	)
{

}



void AGOLMCharacter::UpdateAim()
{
	//DO NOT TOUCH ANYTHING HERE IF YOU DONT KNOW WHAT IS GOOD FOR YOU! IT TOOK ME FOREVER TO GET IT RIGHT

	//Making sure the server does not touch the aim of the player AT ALL

	if (Role != ROLE_Authority || IsLocallyControlled())
	{
		if (GetController() != NULL)
		{

				if (CurrentWeapon != NULL)
				{
					AGOLMPlayerController *playerController = Cast<AGOLMPlayerController>(GetController());

					WeaponMuzzleRotation = CurrentWeapon->WeaponMesh->GetSocketRotation("MuzzleFlash");
					WeaponMuzzleLocation = CurrentWeapon->WeaponMesh->GetSocketLocation("MuzzleFlash");

					FVector startLocation = WeaponMuzzleLocation;

					FVector mouseHit = playerController->GetMouseHit();
					//TracePath(startLocation, mouseHit);
					FRotator aim = (mouseHit - WeaponMuzzleLocation).Rotation();

					FRotator FinalAim;

					//if (bAimPitchable)
						//FinalAim.Roll = -(aim.Pitch + 0.4f);
					//else
						//FinalAim.Roll = 0;

					//FinalAim.Yaw = (aim.Yaw + 10) - GetActorForwardVector().Rotation().Yaw;
					//FinalAim.Pitch = 0;
					WeaponAim = aim;

					if(Role != ROLE_Authority)
						UpdateAim_ServerUpdate(FinalAim);
				}
			
		}
		//Even though THIS WeaponAim is the same name on client and server,
		//the server has no idea what THIS UPDATED CLIENT VALUE is.
		//Once WeaponAim is updated it will be sent to the server to process.
	}
}


void AGOLMCharacter::UpdateAim_ServerUpdate_Implementation(FRotator NewWeaponAim)
{
	//WeaponAim is on the server and NewWeaponAim is from the client, from the client's WeaponAim.
	WeaponAim = NewWeaponAim;
}
bool AGOLMCharacter::UpdateAim_ServerUpdate_Validate(FRotator NewWeaponAim)
{
	return true;
}

void AGOLMCharacter::Equip(EGetWeapon NewWeapon, EEquipSlot In)
{
	if(Role == ROLE_Authority)
	{
		if (CurrentWeapon != NULL)
		{
			CurrentWeapon->Destroy();
		}
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		AWeapon *SpawnedWeapon = NULL;
		
		switch (NewWeapon)
		{
			case EGetWeapon::ROCKET_LAUNCHER: SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(RocketLauncher, spawnParams);	break;
			case EGetWeapon::RIFLE: SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(Rifle, spawnParams); break;
			default: break;
		}
		switch (In)
		{
		case EEquipSlot::HAND_SLOT:
			if (SpawnedWeapon != NULL)
			{
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "RightHandWeaponSocket", EAttachLocation::SnapToTarget);
				bHasHandWeapon = true;
				CurrentWeapon = SpawnedWeapon;
			}
			else
				bHasHandWeapon = false;
			break;
		default:
			break;
		}		
	}
	if (Role != ROLE_Authority)
	{
		ServerEquip(NewWeapon,In);
	}
}

void AGOLMCharacter::GetEquippedWeapons()
{
	if (Role != ROLE_Authority || IsLocallyControlled())
	{
		
		AGOLMPlayerState *PS = Cast<AGOLMPlayerState>(GetController()->PlayerState);
		if (PS)
			Equip(PS->GetWeaponFor(EEquipSlot::HAND_SLOT), EEquipSlot::HAND_SLOT);
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Player State is NULL");
		}
	}
	
	if(Role == ROLE_Authority)
	{
		ClientGetEquippedWeapons();
	}
}

void AGOLMCharacter::ClientGetEquippedWeapons_Implementation()
{
	if(Role != ROLE_Authority)
		GetEquippedWeapons();
}
void AGOLMCharacter::ServerEquip_Implementation(EGetWeapon NewWeapon, EEquipSlot In)
{
	Equip(NewWeapon, In);
}
bool AGOLMCharacter::ServerEquip_Validate(EGetWeapon NewWeapon, EEquipSlot In)
{
	return true;
}

void AGOLMCharacter::ZoomCamera(float deltaZoom)
{
	PlayerCameraBoom->TargetArmLength += deltaZoom;
	PlayerCameraBoom->TargetArmLength = FMath::Clamp(
		PlayerCameraBoom->TargetArmLength,MinCameraHeight,MaxCameraHeight
		);
}

void AGOLMCharacter::FireWeapon()
{
	if (CurrentWeapon != NULL)
	{
		if(Role != ROLE_Authority || IsLocallyControlled())
		{
			if (!CurrentWeapon->bAutomatic)
			{
				bStartShooting = false;
			}

			if(Role != ROLE_Authority)
				ServerFireWeapon();

			bAbleToShoot = false;
			CurrentWeapon->LaunchProjectile(WeaponMuzzleLocation, WeaponMuzzleRotation);
		}
		if(Role == ROLE_Authority)
		{
			bAbleToShoot = false;
			TimeBeforeNextShot = CurrentWeapon->FiringRate;
		}
	}
}
void AGOLMCharacter::ServerFireWeapon_Implementation()
{
	FireWeapon();
}
bool AGOLMCharacter::ServerFireWeapon_Validate()
{
	return true;
}

void AGOLMCharacter::MoveCheck()
{
	if (Role != ROLE_Authority || IsLocallyControlled())
	{
		FVector OverallDirection = FVector::ZeroVector;

		if (bMovingUp || bMovingDown || bMovingRight || bMovingLeft)
			bMoving = true;
		else
			bMoving = false;

		if(bMoving)
		{
			if (bMovingUp)
			{
				OverallDirection += PlayerCamera->GetForwardVector();
			}
			if (bMovingDown)
			{
				OverallDirection += -PlayerCamera->GetForwardVector();
			}

			if (bMovingRight)
			{
				OverallDirection += PlayerCamera->GetRightVector();
			
			}
			if (bMovingLeft)
			{
				OverallDirection += -PlayerCamera->GetRightVector();
				
			}
			OverallDirection.Z = 0;
			OverallDirection.Normalize();
		}
		FinalOrientation = OverallDirection.Rotation();

		if(Role!= ROLE_Authority)
			ServerMove(FinalOrientation,bMoving);
	}
}

void AGOLMCharacter::ServerMove_Implementation(FRotator direction, bool status)
{
	FinalOrientation = direction;
	bMoving = status;
}

bool AGOLMCharacter::ServerMove_Validate(FRotator direction, bool status)
{
	return true;
}


void AGOLMCharacter::Boost(float DeltaTime, bool value)
{
	if(value)
	{
		LaunchCharacter(GetActorUpVector() * BoostSpeed,false,false);
	}

	if (Role != ROLE_Authority)
		ServerBoost(value);
}
void AGOLMCharacter::ServerBoost_Implementation(bool value)
{
	bBoosting = value;
}
bool AGOLMCharacter::ServerBoost_Validate(bool value)
{
	return true;
}

void AGOLMCharacter::Death()
{
	
	if (Role == ROLE_Authority)
	{
		SetRagDoll(true);
		bAlive = false;
		GetCapsuleComponent()->SetCollisionProfileName("NoProjectile");
		ClientDeath();
	}
	else
	{
		GetCapsuleComponent()->SetCollisionProfileName("NoProjectile");
	}

}

void AGOLMCharacter::ServerDeath_Implementation()
{
	Death();
}
bool AGOLMCharacter::ServerDeath_Validate()
{
	return true;
}
void AGOLMCharacter::Respawn()
{
	
	//ClientRespawn();
	//Equip(EGetWeapon::RIFLE, EEquipSlot::HAND_SLOT);
	GetEquippedWeapons();
	bAlive = true;
	RespawnTimeCheck = 0;
	Health = 100;
	GetCapsuleComponent()->SetCollisionProfileName(OriginalCollisionProfile);
	ClientRespawn();
	ClientLoadEntranceLevel("LockerRoom", CurrentLevelStream);
	

}

void AGOLMCharacter::ClientRespawn_Implementation()
{
	if(Role != ROLE_Authority)
	{
		GetCapsuleComponent()->SetCollisionProfileName(OriginalCollisionProfile);
	}
}
bool AGOLMCharacter::ClientRespawn_Validate()
{
	return true;
}

void AGOLMCharacter::GotoLockerRoom()
{
	if(CurrentLevelStream != "LockerRoom")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Green, "Going To locker Room");
		LoadEntranceLevel("LockerRoom", CurrentLevelStream);
	}
}

void AGOLMCharacter::ServerRespawn_Implementation()
{
	Respawn();
}

bool AGOLMCharacter::ServerRespawn_Validate()
{
	return true;
}

void AGOLMCharacter::ClientDeath_Implementation()
{
	if(Role != ROLE_Authority)
		Death();
	//SetRagDoll(true);
}

bool AGOLMCharacter::ClientDeath_Validate()
{
	return true;
}

void AGOLMCharacter::RotateCamera()
{
	AGOLMPlayerController *CharacterController = Cast<AGOLMPlayerController>(Controller);
	float x;
	float y;
	CharacterController->GetInputMouseDelta(x, y);
	FRotator deltaROT = FRotator(0, CameraRotationSensitivity * x, 0);
	PlayerCameraBoom->AddWorldRotation(deltaROT);
}

void AGOLMCharacter::MoveCamera()
{
	AGOLMPlayerController *CharacterController = Cast<AGOLMPlayerController>(Controller);
	float x;
	float y;
	CharacterController->GetInputMouseDelta(x, y);
	FVector CameraFwd = PlayerCameraBoom->GetForwardVector();
	FVector CameraRht = PlayerCameraBoom->GetRightVector();
	CameraFwd.Z = 0;
	CameraRht.Z = 0;

	CameraRht *= (x * CameraMovementSensitivity);
	CameraFwd *= (y * CameraMovementSensitivity);

	FVector deltaMovement = CameraFwd + CameraRht;

	PlayerCameraBoom->TargetOffset += deltaMovement;

}

AWeapon *AGOLMCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

FRotator AGOLMCharacter::GetWeaponAimRotation()
{
	return WeaponAim;
}

void AGOLMCharacter::SetWeaponAimRotation(FRotator deltaROT)
{
	WeaponAim += deltaROT;
}


float AGOLMCharacter::GetPlayerSpeed()
{
	return GetCharacterMovement()->Velocity.Size();
}

void AGOLMCharacter::TracePath(FVector &start, FVector &finish)
{
	DrawDebugLine(GetWorld(), start, finish, FColor::Red,true,2.0f,10);
}

//***When bTearOff is false it will stop the client from replicating and it will send
//it to all other clients that that client has stopped replicating and this function will be called.
void AGOLMCharacter::TornOff()
{
	Super::TornOff();
}

void AGOLMCharacter::Destroyed()
{
	if (CurrentWeapon != NULL)
		CurrentWeapon->Death();
	Super::Destroyed();
}

FName AGOLMCharacter::GetCurrentLevelStream()
{
	return CurrentLevelStream;
}

void AGOLMCharacter::LoadEntranceLevel(FName EntranceLevelName, FName ExitLevelName)
{

	//FLatentActionInfo LatInfo;
	//UGameplayStatics::LoadStreamLevel(GetWorld(), EntranceLevelName, true, true, LatInfo);
	
	MoveToEntrance(EntranceLevelName);
	if(IsLocallyControlled() && Role != ROLE_Authority)
	{
		
		UGameplayStatics::GetStreamingLevel(GetWorld(), EntranceLevelName)->bShouldBeVisible = true;
		if(ExitLevelName != "None" && ExitLevelName != EntranceLevelName)
		{
			UGameplayStatics::GetStreamingLevel(GetWorld(), ExitLevelName)->bShouldBeVisible = false;
		}
	}
	
	
}

void AGOLMCharacter::ClientLoadEntranceLevel_Implementation(FName EntranceLevelName, FName ExitLevelName)
{
	LoadEntranceLevel(EntranceLevelName, ExitLevelName);
}
bool AGOLMCharacter::ClientLoadEntranceLevel_Validate(FName EntranceLevelName, FName ExitLevelName)
{
	return true;
}


void AGOLMCharacter::MoveToEntrance(FName EntranceLevelName)
{
	if (Role == ROLE_Authority)
	{
		if (EntranceLevelName == "LockerRoom")
			ToggleNoCollisionProfile(true);
		else
			ToggleNoCollisionProfile(false);

		CurrentLevelStream = EntranceLevelName;
		Cast<AGOLMGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GotoSpawnLocation(EntranceLevelName, this);
	}
	else
	{
		ServerMoveToEntrance(EntranceLevelName);
	}
}
void AGOLMCharacter::ServerMoveToEntrance_Implementation(FName EntranceLevelName)
{
	MoveToEntrance(EntranceLevelName);
}
bool AGOLMCharacter::ServerMoveToEntrance_Validate(FName EntranceLevelName)
{
	return true;
}


void AGOLMCharacter::SetRagDoll(bool value)
{
	//GetMesh()->SetAllBodiesBelowSimulatePhysics("Base-ZMechPelvis", value);
	if(!IsRunningDedicatedServer())
	{
		GetMesh()->SetSimulatePhysics(value);
		if (value)
		{
			GetMesh()->SetCollisionProfileName("Ragdoll");
			//GetMesh()->WakeAllRigidBodies();
			if (CurrentWeapon != NULL)
				CurrentWeapon->DetachRootComponentFromParent(true);
		}
		else
		{
			GetMesh()->SetCollisionProfileName("CharacterMesh");
			//CapsuleComponent->SetWorldRotation(FRotator(0, 0, 0));
			GetMesh()->AttachTo(GetCapsuleComponent(), NAME_None, EAttachLocation::SnapToTarget, true);
			GetMesh()->AddLocalOffset(FVector(0, 0, 0));
			GetMesh()->AddLocalRotation(FRotator(0, -90, 0));
			//GetMesh()->PutAllRigidBodiesToSleep();
		}
		if (CurrentWeapon != NULL)
			CurrentWeapon->SetRagDoll(value);
	}

	if (Role == ROLE_Authority)
		ClientSetRagDoll(value);
}
void AGOLMCharacter::ClientSetRagDoll_Implementation(bool value)
{
	if (Role != ROLE_Authority)
		SetRagDoll(value);
}

void AGOLMCharacter::RecieveDamage(float damage)
{
	if (Role == ROLE_Authority)
	{
		PlayerState->Health -= damage;
	}
	if (Role != ROLE_Authority)
	{
		ServerRecieveDamage(damage);
	}
}

void AGOLMCharacter::ServerRecieveDamage_Implementation(float damage)
{
	RecieveDamage(damage);
}

bool AGOLMCharacter::ServerRecieveDamage_Validate(float damage)
{
	return true;
}

void AGOLMCharacter::ToggleNoCollisionProfile(bool ToggleOn)
{
	if (ToggleOn)
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->ToggleProjectileCollision(false);
		GetCapsuleComponent()->SetCollisionProfileName(NoPawnCollisionProfile);
	}
	else
	{
		if (CurrentWeapon != NULL)
			CurrentWeapon->ToggleProjectileCollision(true);
		GetCapsuleComponent()->SetCollisionProfileName(OriginalCollisionProfile);
	}


}