// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMGameMode.h"
#include "GOLMLevelStreamBeacon.h"
#include "GOLMEquipmentMenuWidget.h"
#include "GOLMPlayerController.h"
#include "GOLMMiniMapCamera.h"




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
	PlayerCameraBoom->bDoCollisionTest = true;
	

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->AttachTo(PlayerCameraBoom, USpringArmComponent::SocketName);


	
	EquipmentCameraFrontBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FrontCameraBoom"));
	EquipmentCameraFrontBoom->AttachTo(RootComponent);
	EquipmentCameraFront = CreateDefaultSubobject<UCameraComponent>(TEXT("FrontCamera"));
	EquipmentCameraFront->AttachTo(EquipmentCameraFrontBoom, USpringArmComponent::SocketName);


	EquipmentCameraLeftShoulder = CreateDefaultSubobject<UCameraComponent>(TEXT("LeftShoulderCamera"));
	EquipmentCameraLeftShoulderBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("LeftShoulderCameraBoom"));
	EquipmentCameraLeftShoulder->AttachTo(EquipmentCameraLeftShoulderBoom, USpringArmComponent::SocketName);
	EquipmentCameraLeftShoulderBoom->AttachTo(RootComponent);

	EquipmentCameraRightShoulder = CreateDefaultSubobject<UCameraComponent>(TEXT("RightShoulderCamera"));
	EquipmentCameraRightShoulderBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("RightShoulderCameraBoom"));
	EquipmentCameraRightShoulder->AttachTo(EquipmentCameraRightShoulderBoom, USpringArmComponent::SocketName);
	EquipmentCameraRightShoulderBoom->AttachTo(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	
	Health = 100.0f;
	bAlive = true;


	SetReplicates(true);

	//MovementComponent = CreateDefaultSubobject<UGOLMPawnMovementComponent>(TEXT("GOLM Movement Component"));
	//MovementComponent->UpdatedComponent = RootComponent;
}

void AGOLMCharacter::Init()
{

	OriginalCollisionProfile = "Pawn";
	NoPawnCollisionProfile = "NoPawn";

	if (Role == ROLE_Authority)
		ClientInit();

	if (IsLocallyControlled() && Role != ROLE_Authority)
	{
		UWorld *World = GetWorld();
		UGameplayStatics::GetStreamingLevel(World, "LockerRoom")->bShouldBeVisible = false;
		UGameplayStatics::GetStreamingLevel(World, "GameLevel")->bShouldBeVisible = false;
		UGameplayStatics::GetStreamingLevel(World, "Bottomworld")->bShouldBeVisible = false;
	}

}

void AGOLMCharacter::ClientInit_Implementation()
{
	if (Role != ROLE_Authority)
		Init();
}
bool AGOLMCharacter::ClientInit_Validate()
{
	return true;
}

void AGOLMCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = TurnSpeed;
	PlayerCameraBoom->TargetArmLength = MaxCameraHeight / 2;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	
	PlayerCameraActor = GetWorld()->SpawnActor<ACameraActor>(SpawnParams);
	FrontCameraActor = GetWorld()->SpawnActor<ACameraActor>(SpawnParams);
	LeftShoulderCameraActor = GetWorld()->SpawnActor<ACameraActor>(SpawnParams);
	RightShoulderCameraActor = GetWorld()->SpawnActor<ACameraActor>(SpawnParams);

	PlayerCameraActor->CameraComponent = PlayerCamera;
	FrontCameraActor->CameraComponent = EquipmentCameraFront;
	LeftShoulderCameraActor->CameraComponent = EquipmentCameraLeftShoulder;
	RightShoulderCameraActor->CameraComponent = EquipmentCameraRightShoulder;

	RespawnTimeCheck = 0;

	if (GetController() != NULL)
	{
		Cast<AGOLMPlayerController>(GetController())->ShowInGameHud();
	}
	
	DefaultNetCullDistanceSquared = NetCullDistanceSquared;
	bAlive = true;
	Health = 100;
	deathTimer = 5.0f;


	if(MiniMapCamera != NULL)
	{
		MiniMapCameraReference = GetWorld()->SpawnActor<AGOLMMiniMapCamera>(MiniMapCamera.GetDefaultObject()->GetClass(), SpawnParams);
		MiniMapCameraReference->SetPlayerCharacter(this);
	}

	NewCameraHeight = PlayerCameraBoom->TargetArmLength;
}

void AGOLMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGOLMCharacter, CurrentLevelStream);

	DOREPLIFETIME(AGOLMCharacter, WeaponAimPitch);

	DOREPLIFETIME(AGOLMCharacter, RelativeForward);
	DOREPLIFETIME(AGOLMCharacter, RelativeRight);
	DOREPLIFETIME(AGOLMCharacter, FinalOrientation);
	DOREPLIFETIME(AGOLMCharacter, bMoving);

	DOREPLIFETIME(AGOLMCharacter, CurrentHandWeapon);
	DOREPLIFETIME(AGOLMCharacter, CurrentLeftShoulderWeapon);
	DOREPLIFETIME(AGOLMCharacter, CurrentRightShoulderWeapon);
	
	DOREPLIFETIME(AGOLMCharacter, bBoosting);

	DOREPLIFETIME(AGOLMCharacter, Health);
	DOREPLIFETIME(AGOLMCharacter, bAlive);
	DOREPLIFETIME(AGOLMCharacter, TimeUntilRespawn);
	DOREPLIFETIME(AGOLMCharacter, bHasHandWeapon);
}

void AGOLMCharacter::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

}

//***RealViewer refers to player controller and ViewTarget refers to Character
bool AGOLMCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{	
	const AGOLMCharacter *TargetChar = Cast<AGOLMCharacter>(ViewTarget);

	if (TargetChar->CurrentLevelStream == "LockerRoom")
	{
		if (TargetChar == this)
		{
			return true;
		}
		else
			return false;
	}
	else if (TargetChar->CurrentLevelStream == CurrentLevelStream)
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	
	return false;

}


void AGOLMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MiniMapCameraReference != NULL)
		MiniMapCameraReference->UpdateCamera();
	if (IsLocallyControlled())
	{
		//GEngine->ClearOnScreenDebugMessages();
		//if(bAlive)
		//{
		//	
		//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Cyan, "Health: " + FString::SanitizeFloat(Health) );
		//}
		//else
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Respawn In: " + FString::SanitizeFloat(TimeUntilRespawn));
		//}

	}
	
	if (Role != ROLE_Authority || IsLocallyControlled())
	{
		if(bAlive)
		{
			bAimPitchable = GetMovementComponent()->IsFalling();



		}

		if (bRotatingCamera)
			RotateCamera();

	}

	if (Role == ROLE_Authority)
	{
		if (bAlive)
		{
			if (Health <= 0)
			{
				Death();
			}
			
		}
		else
		{
			RespawnTimeCheck += DeltaSeconds;
			TimeUntilRespawn = deathTimer - RespawnTimeCheck;
			if (RespawnTimeCheck > deathTimer)
			{
				
				Respawn();
				
			}
		}
	}

	if (bAlive)
	{

		//if (bHasHandWeapon)
		{
			if (IsLocallyControlled())
			{
				//if(CurrentHandWeapon != NULL)
				{
					;
					UpdateAim(Cast<AGOLMPlayerController>(GetController())->GetMouseHit());
					MoveCamera();
				}
			}

		}

		if (!bMovingUp && !bMovingDown && !bMovingLeft && !bMovingRight)
		{
			bMoving = false;
		}
		else
			bMoving = true;

		if (bMoving)
		{
			MoveCheck();
			if (!bHasHandWeapon)
			{
				SetActorRotation(FMath::Lerp(GetActorForwardVector().Rotation(), FinalOrientation, 0.25f));
				GetCharacterMovement()->AddInputVector(GetActorForwardVector() * (MovingSpeed)* DeltaSeconds);
			}
			else if (bHasHandWeapon)
			{
				GetCharacterMovement()->AddInputVector(FinalOrientation.Vector() * (MovingSpeed)* DeltaSeconds);
			}
			
			
		}
		//MoveCamera(bHasHandWeapon);
		CalculateRelativeDirectionScale();
		if (bBoosting)
			Boost();
	}
	if (PlayerCameraBoom->TargetArmLength != NewCameraHeight)
	{
		PlayerCameraBoom->TargetArmLength = FMath::Lerp<float>(PlayerCameraBoom->TargetArmLength, NewCameraHeight, 0.1);
	}

	//if (!bRotatingCamera)
	{
		if (PlayerCameraBoom->TargetOffset != NewCameraOffset)
		{
			PlayerCameraBoom->TargetOffset = FMath::Lerp<FVector>(PlayerCameraBoom->TargetOffset, NewCameraOffset, 0.05);
		}
	}
}

void AGOLMCharacter::NotifyHit
(class UPrimitiveComponent * MyComp,	AActor * Other,class UPrimitiveComponent * OtherComp,	bool bSelfMoved,
	FVector HitLocation,	FVector HitNormal,	FVector NormalImpulse,	const FHitResult & Hit
	)
{

}



void AGOLMCharacter::UpdateAim(FVector MouseHit)
{
	
	if (CurrentHandWeapon != NULL)
	{

		//FRotator aim = (MouseHit - GetMesh()->GetSocketLocation("SpineSocket")).Rotation();
		FRotator aim = (MouseHit - CurrentHandWeapon->WeaponMesh->GetSocketLocation("MuzzleFlash")).Rotation();
		//FRotator aim = MouseHit.Rotation();
		SetActorRotation(FRotator(0, aim.Yaw, 0));

		FRotator WeaponRot = FRotator(WeaponAimPitch, aim.Yaw, -180);
		CurrentHandWeapon->SetActorRotation(WeaponRot);

		float NewPitch = CurrentHandWeapon->CalculateProjectilePath(MouseHit);

		if (NewPitch != 0)
			WeaponAimPitch = NewPitch;

		if (bIsInMenu)
			WeaponAimPitch = -30;






		HandSupportLocation = CurrentHandWeapon->WeaponMesh->GetSocketLocation("HandSupport");
		GripBoneLocation = CurrentHandWeapon->WeaponMesh->GetBoneLocation("Grip_Bone");
		LeftPalmLocation = GetMesh()->GetSocketLocation("LeftPalmSocket");




		if (Role != ROLE_Authority)
			ServerUpdateAim(WeaponAimPitch, aim.Yaw);
	}

	if (CurrentLeftShoulderWeapon != NULL)
	{
		FRotator LSWorldRot;
		FVector LSWorldPOS;
		CurrentLeftShoulderWeapon->WeaponMesh->GetSocketWorldLocationAndRotation("MuzzleFlash", LSWorldPOS, LSWorldRot);
		FRotator Direction = (MouseHit - LSWorldPOS).Rotation();

		float NewPitch = CurrentLeftShoulderWeapon->CalculateProjectilePath(MouseHit);



		CurrentLeftShoulderWeapon->SetActorRotation(FRotator(NewPitch,Direction.Yaw,0));
	}

	if (CurrentRightShoulderWeapon != NULL)
	{
		FRotator LSWorldRot;
		FVector LSWorldPOS;
		CurrentRightShoulderWeapon->WeaponMesh->GetSocketWorldLocationAndRotation("MuzzleFlash", LSWorldPOS, LSWorldRot);
		FRotator Direction = (MouseHit - LSWorldPOS).Rotation();

		float NewPitch = CurrentRightShoulderWeapon->CalculateProjectilePath(MouseHit);


		if (bIsInMenu)
			NewPitch = 70;

		CurrentRightShoulderWeapon->SetActorRotation(FRotator(NewPitch,Direction.Yaw,0));
	}
}


void AGOLMCharacter::ServerUpdateAim_Implementation(float NewPitch, float NewYaw)
{
	WeaponAimPitch = NewPitch;
	SetActorRotation(FRotator(0, NewYaw, 0));
}
bool AGOLMCharacter::ServerUpdateAim_Validate(float NewPitch, float NewYaw)
{
	return true;
}




void AGOLMCharacter::Equip(AWeapon *NewWeapon, EEquipSlot In)
{
	if(Role == ROLE_Authority)
	{

		
	
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		AWeapon *SpawnedWeapon = NULL;
		//SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon, spawnParams);
		if (NewWeapon != NULL)
		{
			SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon->GetClass(), spawnParams);
		}
		switch (In)
		{
		case EEquipSlot::HAND_SLOT:
			if (CurrentHandWeapon != NULL)
			{
				CurrentHandWeapon->Destroy();
			}

			if (SpawnedWeapon != NULL)
			{
				SpawnedWeapon->WeaponMesh->SetWorldRotation(FRotator(0, 90, -180));
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "HandWeaponSock", EAttachLocation::SnapToTarget);
				SpawnedWeapon->CurrentSlotType = In;
				bHasHandWeapon = true;
				CurrentHandWeapon = SpawnedWeapon;
			}
			else
			{
				CurrentHandWeapon = NULL;
				bHasHandWeapon = false;
			}
			break;
		case EEquipSlot::LEFT_SHOULDER:
			if (CurrentLeftShoulderWeapon != NULL)
			{
				CurrentLeftShoulderWeapon->Destroy();
			}

			if (SpawnedWeapon != NULL)
			{

				SpawnedWeapon->SetActorScale3D(FVector(0.75, 0.45, 0.75));
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "LeftShoulderWeaponSock", EAttachLocation::SnapToTarget);
				SpawnedWeapon->CurrentSlotType = In;
				CurrentLeftShoulderWeapon = SpawnedWeapon;
			}
			else
			{
				CurrentLeftShoulderWeapon = NULL;
			}
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			if (CurrentRightShoulderWeapon != NULL)
			{
				CurrentRightShoulderWeapon->Destroy();
			}
			if (SpawnedWeapon != NULL)
			{

				//SpawnedWeapon->WeaponMesh->SetWorldRotation(FRotator(0,0,0));
				SpawnedWeapon->SetActorScale3D(FVector(0.5, 0.25, 0.5));
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "RightShoulderWeaponSock", EAttachLocation::SnapToTarget);
				SpawnedWeapon->CurrentSlotType = In;
				CurrentRightShoulderWeapon = SpawnedWeapon;
				//CurrentRightShoulderWeapon->bArcFire = true;
			}
			else
			{
				CurrentRightShoulderWeapon = NULL;
			}
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
		AGOLMPlayerState *PS = NULL;
		if(GetController() != NULL)
			 PS = Cast<AGOLMPlayerState>(GetController()->PlayerState);
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Player Controller is NULL from Character");

		if (PS!=NULL)
		{
			Equip(PS->GetWeaponFor(EEquipSlot::HAND_SLOT), EEquipSlot::HAND_SLOT);
			Equip(PS->GetWeaponFor(EEquipSlot::LEFT_SHOULDER), EEquipSlot::LEFT_SHOULDER);
			Equip(PS->GetWeaponFor(EEquipSlot::RIGHT_SHOULDER), EEquipSlot::RIGHT_SHOULDER);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Player State is NULL from Character");
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
void AGOLMCharacter::ServerEquip_Implementation(AWeapon *NewWeapon, EEquipSlot In)
{
	Equip(NewWeapon, In);
}
bool AGOLMCharacter::ServerEquip_Validate(AWeapon *NewWeapon, EEquipSlot In)
{
	return true;
}

void AGOLMCharacter::ZoomMiniMapCamera(float value)
{
	if (MiniMapCameraReference != NULL)
	{
		MiniMapCameraReference->Zoom(value);
	}
}

void AGOLMCharacter::FireHandWeapon(bool value)
{
	if (CurrentHandWeapon != NULL)
		CurrentHandWeapon->bStartShooting = value;
}

void AGOLMCharacter::FireLeftShoulderWeapon(bool value)
{
	if (CurrentLeftShoulderWeapon != NULL)
		CurrentLeftShoulderWeapon->bStartShooting = value;
}
void AGOLMCharacter::FireRightShoulderWeapon(bool value)
{
	if (CurrentRightShoulderWeapon != NULL)
		CurrentRightShoulderWeapon->bStartShooting = value;
}

void AGOLMCharacter::MoveCheck()
{
	if (Role != ROLE_Authority || IsLocallyControlled())
	{
		FVector OverallDirection = FVector::ZeroVector;

		if (bMovingUp)		OverallDirection += PlayerCamera->GetForwardVector();
		if (bMovingDown)	OverallDirection += -PlayerCamera->GetForwardVector();
		if (bMovingRight)	OverallDirection += PlayerCamera->GetRightVector();
		if (bMovingLeft)	OverallDirection += -PlayerCamera->GetRightVector();
			
		OverallDirection.Z = 0;
		OverallDirection.Normalize();

		FinalOrientation = OverallDirection.Rotation();

		

		if (Role != ROLE_Authority)
		{
			ServerMove(FinalOrientation);
		}
	}
}

void AGOLMCharacter::ServerMove_Implementation(FRotator direction)
{
	FinalOrientation = direction;
}
bool AGOLMCharacter::ServerMove_Validate(FRotator direction){return true;}



void AGOLMCharacter::Boost()
{
	//if (Role != ROLE_Authority || IsLocallyControlled())
	{
		FVector direction = FVector::ZeroVector;

		{
			if (bMovingUp)		direction += PlayerCamera->GetForwardVector();
			if (bMovingDown)	direction += -PlayerCamera->GetForwardVector();
			if (bMovingRight)	direction += PlayerCamera->GetRightVector();
			if (bMovingLeft)	direction += -PlayerCamera->GetRightVector();

			direction.Z = 0;

			if (direction != FVector::ZeroVector)
			{
				//if (Role != ROLE_Authority)
				{
					if (direction != FVector::ZeroVector)
						BoostCharacter(direction * BoostSpeed);
				}
			}
		}
	}
}

void AGOLMCharacter::ServerBoost_Implementation(FVector LaunchDirection)
{
	BoostCharacter(LaunchDirection);
}
bool AGOLMCharacter::ServerBoost_Validate(FVector LaunchDirection)
{
	return true;
}

void AGOLMCharacter::BoostCharacter(FVector LaunchVelocity)
{
	LaunchCharacter(LaunchVelocity.GetSafeNormal() * BoostSpeed, false, false);
	if (!bHasHandWeapon)
		SetActorRotation(LaunchVelocity.GetSafeNormal().Rotation());


	

	if (Role != ROLE_Authority)
	{
		ServerBoost(LaunchVelocity);
	}
	
	if (Role == ROLE_Authority)
	{
		//ClientBoostCharacter(LaunchVelocity);
	}
}

void AGOLMCharacter::ClientBoostCharacter_Implementation(FVector NewLaunchVelocity)
{
	if (Role != ROLE_Authority)
	{
		BoostCharacter(NewLaunchVelocity);
		//LaunchCharacter(NewLaunchVelocity.GetSafeNormal() * BoostSpeed, false, false);
		//if (!bHasHandWeapon)
		//	SetActorRotation(NewLaunchVelocity.GetSafeNormal().Rotation());
	}
}

void AGOLMCharacter::Respawn()
{
	
	if(Role == ROLE_Authority)
	{
		if (!bAlive)
		{
			bAlive = true;
			SetRagDoll(false);
		}
		RespawnTimeCheck = 0;
		Health = 100;
		//ChangePlayerCollisionProfile(NoPawnCollisionProfile);
		LoadEntranceLevel("LockerRoom");
		GetEquippedWeapons();
	}
	
}

void AGOLMCharacter::ClientRespawn_Implementation()
{
	if(Role != ROLE_Authority)
	{
		
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
		LoadEntranceLevel("LockerRoom");
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
void AGOLMCharacter::Death()
{
	SetPawnCollisionType(EPlayerCollisionProfile::DEATH);
	if (Role == ROLE_Authority)
	{
		SetRagDoll(true);
		bAlive = false;
		ClientDeath();
	}
	
}

void AGOLMCharacter::ClientDeath_Implementation()
{
	if(Role != ROLE_Authority)
	{
		bAlive = false;
		Death();
	}

}

bool AGOLMCharacter::ClientDeath_Validate()
{
	return true;
}

void AGOLMCharacter::RotateCamera()
{
	AGOLMPlayerController *CharacterController = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	float x;
	float y;
	CharacterController->GetInputMouseDelta(x, y);
	FRotator deltaROT = FRotator(0, CameraRotationSensitivity * x, 0);
	PlayerCameraBoom->AddWorldRotation(deltaROT);
}

void AGOLMCharacter::MoveCamera()
{


	AGOLMPlayerController *PlayerCon = Cast<AGOLMPlayerController>(GetController());
	//FVector CameraCheck = PlayerCameraBoom->TargetOffset + FVector(DeltaMouse.X * CameraMovementSensitivity, DeltaMouse.Y * CameraMovementSensitivity, 0);
	FVector MouseWorldPOS;
	FVector MouseWorldDirection;
	PlayerCon->DeprojectMousePositionToWorld(MouseWorldPOS, MouseWorldDirection);
	
	FVector Intersection = FMath::LinePlaneIntersection(PlayerCon->PlayerCameraManager->GetCameraLocation(), MouseWorldPOS, GetActorLocation(), FVector::UpVector);

	FVector CameraCheck = Intersection - GetActorLocation();
	if (CameraCheck.Size()<MaxCameraMoveRadius)
	{
		NewCameraOffset = CameraCheck;
		NewCameraOffset.Z = 0;
	}
	else
	{
		NewCameraOffset = CameraCheck.GetSafeNormal() * MaxCameraMoveRadius;
		NewCameraOffset.Z = 0;
	}
}

void AGOLMCharacter::ZoomCamera(float DeltaHeight)
{
	NewCameraHeight = PlayerCameraBoom->TargetArmLength + DeltaHeight;
	NewCameraHeight = FMath::Clamp<float>(NewCameraHeight, MinCameraHeight, MaxCameraHeight);
}

AWeapon *AGOLMCharacter::GetCurrentWeapon()
{
	return CurrentHandWeapon;
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
	if (CurrentHandWeapon != NULL)
		CurrentHandWeapon->Death();

	if (CurrentLeftShoulderWeapon != NULL)
		CurrentLeftShoulderWeapon->Death();

	if (CurrentRightShoulderWeapon != NULL)
		CurrentRightShoulderWeapon->Death();
	Super::Destroyed();
}

FName AGOLMCharacter::GetCurrentLevelStream()
{
	return CurrentLevelStream;
}

void AGOLMCharacter::LoadEntranceLevel(FName EntranceLevelName)
{
	if (IsLocallyControlled() && Role != ROLE_Authority)
	{

		UGameplayStatics::GetStreamingLevel(GetWorld(), EntranceLevelName)->bShouldBeVisible = true;
		if (CurrentLevelStream != "None" && CurrentLevelStream != EntranceLevelName)
			UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentLevelStream)->bShouldBeVisible = false;

	}

	if (IsLocallyControlled() || Role != ROLE_Authority)
		MoveToEntrance(EntranceLevelName);

	if (Role == ROLE_Authority)
	{
		ClientLoadEntranceLevelNameOnly(EntranceLevelName);
	}
}

void AGOLMCharacter::ClientLoadEntranceLevelNameOnly_Implementation(FName EntranceLevelName)
{
	if (Role != ROLE_Authority)
		LoadEntranceLevel(EntranceLevelName);
}
bool AGOLMCharacter::ClientLoadEntranceLevelNameOnly_Validate(FName EntranceLevelName)
{
	return true;
}


void AGOLMCharacter::LoadEntranceLevel(AGOLMLevelStreamBeacon *LevelBeacon)
{
	if (IsLocallyControlled() && Role != ROLE_Authority)
	{
		
		UGameplayStatics::GetStreamingLevel(GetWorld(), LevelBeacon->NameOfLevelToLoad)->bShouldBeVisible = true;
		if (CurrentLevelStream != "None" && CurrentLevelStream != LevelBeacon->NameOfLevelToLoad)
			UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentLevelStream)->bShouldBeVisible = false;

	}

	if (IsLocallyControlled() || Role != ROLE_Authority)
		MoveToEntrance(LevelBeacon);

	if (Role == ROLE_Authority)
	{
		ClientLoadEntranceLevel(LevelBeacon);
	}
}

void AGOLMCharacter::ClientLoadEntranceLevel_Implementation(AGOLMLevelStreamBeacon *LevelBeacon)
{
	if(Role!= ROLE_Authority)
		LoadEntranceLevel(LevelBeacon);
}
bool AGOLMCharacter::ClientLoadEntranceLevel_Validate(AGOLMLevelStreamBeacon *LevelBeacon)
{
	return true;
}


void AGOLMCharacter::MoveToEntrance(FName EntranceLevelNameTag)
{
	if (Role == ROLE_Authority)
	{
		if (EntranceLevelNameTag == "LockerRoom")
			SetPawnCollisionType(EPlayerCollisionProfile::LOCKER);
		else
			SetPawnCollisionType(EPlayerCollisionProfile::REGULAR);

		
		TArray<AActor*, FDefaultAllocator> SpawnLocs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGOLMPlayerStart::StaticClass(), SpawnLocs);
		
		//Looking for a start location to teleport to
		if (SpawnLocs.Num() != 0)
		{
			for (int32 i = 0; i < SpawnLocs.Num(); i++)
			{
				AGOLMPlayerStart *StartLocation = Cast<AGOLMPlayerStart>(SpawnLocs[i]);
				if (StartLocation->PlayerStartTag == EntranceLevelNameTag)
				{
					//SetActorLocation(StartLocation->GetSpawnLocation());
					CurrentLevelStream = EntranceLevelNameTag;
					
					TeleportTo(StartLocation->GetSpawnLocation(), GetActorRotation());
					
					break;
				}
			}
			
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "NO SPAWN LOCATIONS");
	}
	
	if (Role != ROLE_Authority)
	{
		if (Role != ROLE_Authority)
			ServerMoveToEntranceTagOnly(EntranceLevelNameTag);
	}
}
void AGOLMCharacter::ServerMoveToEntranceTagOnly_Implementation(FName EntranceLevelName)
{
	MoveToEntrance(EntranceLevelName);
}
bool AGOLMCharacter::ServerMoveToEntranceTagOnly_Validate(FName EntranceLevelName)
{
	return true;
}


void AGOLMCharacter::MoveToEntrance(AGOLMLevelStreamBeacon *LevelBeacon)
{
	if (Role == ROLE_Authority)
	{
		if (LevelBeacon->NameOfLevelToLoad == "LockerRoom")
			SetPawnCollisionType(EPlayerCollisionProfile::LOCKER);
		else
			SetPawnCollisionType(EPlayerCollisionProfile::REGULAR);


		TArray<AActor*, FDefaultAllocator> SpawnLocs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGOLMPlayerStart::StaticClass(), SpawnLocs);

		//Looking for a start location to teleport to
		if (SpawnLocs.Num() != 0)
		{
			for (int32 i = 0; i < SpawnLocs.Num(); i++)
			{
				AGOLMPlayerStart *StartLocation = Cast<AGOLMPlayerStart>(SpawnLocs[i]);
				if (StartLocation->PlayerStartTag == LevelBeacon->PlayStartLocationTag)
				{
					//SetActorLocation(SpawnLocs[i]->GetActorLocation());
					//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), StartLocation->GetSpawnLocation().ToString());
					TeleportTo(StartLocation->GetSpawnLocation(), GetActorRotation());
					break;
				}
			}
			CurrentLevelStream = LevelBeacon->NameOfLevelToLoad;
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "NO SPAWN LOCATIONS");
	}

	if (Role != ROLE_Authority)
	{
		if (Role != ROLE_Authority)
			ServerMoveToEntrance(LevelBeacon);
	}
}
void AGOLMCharacter::ServerMoveToEntrance_Implementation(AGOLMLevelStreamBeacon *LevelBeacon)
{
	MoveToEntrance(LevelBeacon);
}
bool AGOLMCharacter::ServerMoveToEntrance_Validate(AGOLMLevelStreamBeacon *LevelBeacon)
{
	return true;
}



void AGOLMCharacter::SetRagDoll(bool value)
{
	//if(!IsRunningDedicatedServer())
	
		if(!IsRunningDedicatedServer())
		{
			
			GetMesh()->SetSimulatePhysics(value);
			if (value)
			{
				
				GetMesh()->SetCollisionProfileName("Ragdoll");
				if (CurrentHandWeapon != NULL)
				{
					CurrentHandWeapon->SetRagDoll(value);
					CurrentHandWeapon->DetachRootComponentFromParent(true);
					
				}
				if (CurrentLeftShoulderWeapon != NULL)
				{
					CurrentLeftShoulderWeapon->SetRagDoll(value);
					CurrentLeftShoulderWeapon->DetachRootComponentFromParent(true);

				}
				if (CurrentRightShoulderWeapon != NULL)
				{
					CurrentRightShoulderWeapon->SetRagDoll(value);
					CurrentRightShoulderWeapon->DetachRootComponentFromParent(true);

				}
			}
			else
			{
				
				GetMesh()->SetCollisionProfileName("CharacterMesh");
				GetMesh()->AttachTo(GetCapsuleComponent(), NAME_None, EAttachLocation::SnapToTarget, true);
				GetMesh()->AddLocalOffset(FVector(0, 0, 0));
				GetMesh()->AddLocalRotation(FRotator(0, -90, 0));
			}
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

void AGOLMCharacter::CalculateRelativeDirectionScale()
{

	if(IsLocallyControlled())
	{
		if(bMoving)
		{
			FVector FacingVec = GetActorForwardVector();
			FacingVec.Z = 0;
			FVector MovementVec = FinalOrientation.Vector();
			MovementVec.Z = 0;
			FVector RightFacingVec = GetActorRightVector();
			RightFacingVec.Z = 0;

			RelativeForward = FVector::DotProduct(FacingVec, MovementVec);
			RelativeRight = FVector::DotProduct(RightFacingVec, MovementVec);
		}
		else
		{
			RelativeForward = RelativeRight = 0;
		}
	}
	if (Role != ROLE_Authority)
		ServerUpdateRelativeDirectionScale(RelativeForward, RelativeRight);
}


void AGOLMCharacter::ServerUpdateRelativeDirectionScale_Implementation(float relativeforwardscale, float relativerightscale)
{
	RelativeForward = relativeforwardscale;
	RelativeRight = relativerightscale;
}
bool AGOLMCharacter::ServerUpdateRelativeDirectionScale_Validate(float relativeforwardscale, float relativerightscale)
{
	return true;
}

void AGOLMCharacter::SetPawnCollisionType(EPlayerCollisionProfile NewCollision)
{
	switch (NewCollision)
	{
	case EPlayerCollisionProfile::LOCKER:
		GetCapsuleComponent()->SetCollisionProfileName("PlayerIgnoreAll");
		//GetCapsuleComponent()->SetCollisionProfileName("Pawn");
		break;
	case EPlayerCollisionProfile::DEATH:
		GetCapsuleComponent()->SetCollisionProfileName("PlayerIgnoreAll");
		//GetCapsuleComponent()->SetCollisionProfileName("Pawn");
		break;
	case EPlayerCollisionProfile::REGULAR:
		GetCapsuleComponent()->SetCollisionProfileName("Player");
		break;
	default:
		break;
	}
	if (Role == ROLE_Authority)
		ClientSetPawnCollisionType(NewCollision);

	if (Role != ROLE_Authority && IsLocallyControlled())
		ServerSetPawnCollisionType(NewCollision);
}
void AGOLMCharacter::ServerSetPawnCollisionType_Implementation(EPlayerCollisionProfile NewCollisionType)
{
	SetPawnCollisionType(NewCollisionType);
}
bool AGOLMCharacter::ServerSetPawnCollisionType_Validate(EPlayerCollisionProfile NewCollisionType)
{
	return true;
}
void AGOLMCharacter::ClientSetPawnCollisionType_Implementation(EPlayerCollisionProfile NewCollisionType)
{
	if (Role != ROLE_Authority)
		SetPawnCollisionType(NewCollisionType);
}

void AGOLMCharacter::OnRep_ShowCompatibleProjectiles()
{
	if (IsLocallyControlled())
	{
		AGOLMPlayerController *PlayerCon = Cast<AGOLMPlayerController>(GetController());
		if(PlayerCon != NULL)
			if (PlayerCon->bIsInEquipmentMenu)
			{
				Cast<UGOLMEquipmentMenuWidget>(PlayerCon->EquipmentMenuReference)->SetupWeaponProjectileSelection();
			}
	}
}

//***Done on server side
float AGOLMCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Damage > 0.0f)
	{

		UDamageType *DamageType = Cast<UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
		Health -= Damage;

	}
	return Damage;
}