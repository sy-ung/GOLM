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

	PrimaryActorTick.bCanEverTick = true;

	Health = 1000.0f;
	bAlive = true;
	SetReplicates(true);

	if (bIsAI)
		return;

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
		//UGameplayStatics::GetStreamingLevel(World, "LockerRoom")->bShouldBeVisible = false;
		//UGameplayStatics::GetStreamingLevel(World, "GameLevel")->bShouldBeVisible = false;
		//UGameplayStatics::GetStreamingLevel(World, "Bottomworld")->bShouldBeVisible = false;
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

	bAlive = true;
	Health = 100;
	deathTimer = 5.0f;


	if (bIsAI)
		return;

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

	if (Cast<AGOLMPlayerController>(GetController()) != NULL)
	{
		Cast<AGOLMPlayerController>(GetController())->ShowInGameHud();
	}



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

	DOREPLIFETIME(AGOLMCharacter, FinalOrientation);
	DOREPLIFETIME(AGOLMCharacter, bMoving);

	DOREPLIFETIME(AGOLMCharacter, CurrentHandWeapon);
	DOREPLIFETIME(AGOLMCharacter, CurrentLeftShoulderWeapon);
	DOREPLIFETIME(AGOLMCharacter, CurrentRightShoulderWeapon);
	DOREPLIFETIME(AGOLMCharacter, TargetLocation);
	
	DOREPLIFETIME(AGOLMCharacter, bBoosting);

	DOREPLIFETIME(AGOLMCharacter, Health);
	DOREPLIFETIME(AGOLMCharacter, bAlive);
	DOREPLIFETIME(AGOLMCharacter, TimeUntilRespawn);

	DOREPLIFETIME(AGOLMCharacter, CharacterName);

	DOREPLIFETIME(AGOLMCharacter, ReturnHomeTimer);
	DOREPLIFETIME(AGOLMCharacter, bCanGoHome);
}

void AGOLMCharacter::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

}

//***RealViewer refers to player controller and ViewTarget refers to Character
bool AGOLMCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	if (Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation))
	{
		return true;
	}
	else
	{
		return false;
	}
		

}


void AGOLMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	//****NON-Network updates - Runs on client and server simultaneously
	UpdateAim();

	if (bAlive)
	{


		CalculateRelativeDirectionScale();
		if (bBoosting)
			Boost();

		if (bMoving)
		{
			//MoveCheck();

			if (!UKismetSystemLibrary::IsValid(CurrentHandWeapon))
			{
				SetActorRotation(FMath::Lerp(GetActorForwardVector().Rotation(), FinalOrientation, 0.25f));
				
				if(!bIsAI)
					GetCharacterMovement()->AddInputVector(GetActorForwardVector() * (MovingSpeed)* DeltaSeconds);
			}
			else
			{
				if (!bIsAI)
					GetCharacterMovement()->AddInputVector(FinalOrientation.Vector() * (MovingSpeed)* DeltaSeconds);
			}
		}

	}




	//****Updates on client side that will be sent to server
	if (Role != ROLE_Authority || IsLocallyControlled())
	{

	}

	//****Updates on server side that will replicate to client
	if (Role == ROLE_Authority)
	{
		if (bAlive)
		{
			if (Health <= 0)
			{
				Death();
				bMoving = false;
			}
			
		}
		else
		{
			RespawnTimeCheck += DeltaSeconds;
			TimeUntilRespawn = deathTimer - RespawnTimeCheck;
			if (RespawnTimeCheck > deathTimer)
			{
				if (bIsAI)
					Destroy();
				else
					Respawn();
			}
		}
	}


	if (bIsAI)
		return;




	////****Locally Controlled Updates - Values to get from local or things to only run on local
	//GEngine->ClearOnScreenDebugMessages();
	//if (bAlive)
	//{

	//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Cyan, "Health: " + FString::SanitizeFloat(Health));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Respawn In: " + FString::SanitizeFloat(TimeUntilRespawn));
	//}

	if(bAlive)
	{
		if (IsLocallyControlled())
		{
			if (MiniMapCameraReference != NULL)
				MiniMapCameraReference->UpdateCamera();


			if (bRotatingCamera)
				RotateCamera();

			
			if(!bCanGoHome)
			{
				if (bStartHomeTimer)
				{
					ReturnHomeTimer += DeltaSeconds;
					if (ReturnHomeTimer >= ReturnHomeCoolDown)
					{
						bCanGoHome = true;
					}
				}
			}
				



	
			UpdateTargetLocation(Cast<AGOLMPlayerController>(GetController())->GetMouseHit());
			MoveCamera();

			MoveCheck();

			if (PlayerCameraBoom->TargetOffset != NewCameraOffset)
			{
				PlayerCameraBoom->TargetOffset = FMath::Lerp<FVector>(PlayerCameraBoom->TargetOffset, NewCameraOffset, 0.05);
			}

			if (PlayerCameraBoom->TargetArmLength != NewCameraHeight)
			{
				PlayerCameraBoom->TargetArmLength = FMath::Lerp<float>(PlayerCameraBoom->TargetArmLength, NewCameraHeight, 0.1);
			}

		}
	}

}

void AGOLMCharacter::NotifyHit
(class UPrimitiveComponent * MyComp,	AActor * Other,class UPrimitiveComponent * OtherComp,	bool bSelfMoved,
	FVector HitLocation,	FVector HitNormal,	FVector NormalImpulse,	const FHitResult & Hit
	)
{

}

void AGOLMCharacter::UpdateTargetLocation(FVector NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
	if (Role != ROLE_Authority)
		ServerUpdateTargetLocation(NewTargetLocation);
}

void AGOLMCharacter::ServerUpdateTargetLocation_Implementation(FVector NewTargetLocation)
{
	UpdateTargetLocation(NewTargetLocation);
}

bool AGOLMCharacter::ServerUpdateTargetLocation_Validate(FVector NewTargetLocation)
{
	return true;
}

void AGOLMCharacter::UpdateAim()
{
	

	if (CurrentHandWeapon != NULL)
	{

		FRotator aim = (TargetLocation - CurrentHandWeapon->WeaponMesh->GetSocketLocation("MuzzleFlash")).Rotation();
		SetActorRotation(FRotator(0, aim.Yaw, 0));

		FRotator WeaponRot = FRotator(CurrentHandWeaponAimPitch, aim.Yaw, -180);
		CurrentHandWeapon->SetActorRotation(WeaponRot);
		
		float NewPitch = 0;

		if (CurrentHandWeapon->CurrentProjectile->MovementComponent->ProjectileGravityScale == 0)
			NewPitch = aim.Pitch;
		else
			NewPitch = CurrentHandWeapon->CalculateProjectilePath(TargetLocation);

		if (NewPitch != 0)
			CurrentHandWeaponAimPitch = NewPitch;

		if (bIsInMenu)
			CurrentHandWeaponAimPitch = -30;

		HandSupportLocation = CurrentHandWeapon->WeaponMesh->GetSocketLocation("HandSupport");
		GripBoneLocation = CurrentHandWeapon->WeaponMesh->GetBoneLocation("Grip_Bone");
		LeftPalmLocation = GetMesh()->GetSocketLocation("LeftPalmSocket");

	}

	if (CurrentLeftShoulderWeapon != NULL)
	{
		FRotator LSWorldRot;
		FVector LSWorldPOS;
		CurrentLeftShoulderWeapon->WeaponMesh->GetSocketWorldLocationAndRotation("MuzzleFlash", LSWorldPOS, LSWorldRot);
		FRotator Direction = (TargetLocation - LSWorldPOS).Rotation();

		float NewPitch = 0;

		if (CurrentLeftShoulderWeapon->CurrentProjectile->MovementComponent->ProjectileGravityScale == 0)
			NewPitch = Direction.Pitch;
		else
			NewPitch = CurrentLeftShoulderWeapon->CalculateProjectilePath(TargetLocation);

		CurrentLeftShoulderWeapon->SetActorRotation(FRotator(NewPitch,Direction.Yaw,0));
		
	}

	if (CurrentRightShoulderWeapon != NULL)
	{
		FRotator LSWorldRot;
		FVector LSWorldPOS;
		CurrentRightShoulderWeapon->WeaponMesh->GetSocketWorldLocationAndRotation("MuzzleFlash", LSWorldPOS, LSWorldRot);
		FRotator Direction = (TargetLocation - LSWorldPOS).Rotation();

		float NewPitch = 0;

		if (CurrentRightShoulderWeapon->CurrentProjectile->MovementComponent->ProjectileGravityScale == 0)
			NewPitch = Direction.Pitch;
		else
			NewPitch = CurrentRightShoulderWeapon->CalculateProjectilePath(TargetLocation);


		if (bIsInMenu)
			NewPitch = 70;

		CurrentRightShoulderWeapon->SetActorRotation(FRotator(NewPitch,Direction.Yaw,0));
	}
}

void AGOLMCharacter::Equip(AWeapon *NewWeapon, EEquipSlot In)
{
	if(Role == ROLE_Authority)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		AWeapon *SpawnedWeapon = NULL;
		//SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon, spawnParams);

		if (bIsAI)
		{
			if (NewWeapon != NULL)
				SpawnedWeapon = NewWeapon;
			else
				SpawnedWeapon = NULL;
		}
		//else if (NewWeapon != NULL)
		//{
		//	SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon->GetClass(), spawnParams);
		//}

		switch (In)
		{
		case EEquipSlot::HAND_SLOT:
			//Mainly for AI
			if (SpawnedWeapon != NULL)
			{
				CurrentHandWeapon = SpawnedWeapon;
				SpawnedWeapon->WeaponMesh->SetWorldRotation(FRotator(0, 90, -180));
				CurrentHandWeapon->AttachRootComponentTo(GetMesh(), "HandWeaponSock", EAttachLocation::SnapToTarget);
			}
			else if(NewWeapon != NULL)
			{
				if(CurrentHandWeapon !=NULL)
					CurrentHandWeapon->Destroy();

				SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon->GetClass(), spawnParams);
				SpawnedWeapon->WeaponMesh->SetWorldRotation(FRotator(0, 90, -180));
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "HandWeaponSock", EAttachLocation::SnapToTarget);
					
				SpawnedWeapon->CurrentSlotType = In;
				CurrentHandWeapon = SpawnedWeapon;
				if (NewWeapon->CurrentProjectile != NULL)
					CurrentHandWeapon->CurrentProjectile = NewWeapon->CurrentProjectile;
			}
			else
			{
				if(CurrentHandWeapon!=NULL)
					CurrentHandWeapon->Destroy();
				CurrentHandWeapon = NULL;
			}
			break;
		case EEquipSlot::LEFT_SHOULDER:
			//Mainly for AI
			if (SpawnedWeapon != NULL)
			{
				CurrentLeftShoulderWeapon = SpawnedWeapon;
				CurrentLeftShoulderWeapon->AttachRootComponentTo(GetMesh(), "LeftShoulderWeaponSock", EAttachLocation::SnapToTarget);
			}
			else if (NewWeapon != NULL)
			{
				if (CurrentLeftShoulderWeapon != NULL)
					CurrentLeftShoulderWeapon->Destroy();

				SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon->GetClass(), spawnParams);
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "LeftShoulderWeaponSock", EAttachLocation::SnapToTarget);

				SpawnedWeapon->CurrentSlotType = In;
				SpawnedWeapon->SetActorScale3D(FVector(0.75, 0.45, 0.75));
				CurrentLeftShoulderWeapon = SpawnedWeapon;
				if (NewWeapon->CurrentProjectile != NULL)
					CurrentLeftShoulderWeapon->CurrentProjectile = NewWeapon->CurrentProjectile;
			}
			else
			{
				if(CurrentLeftShoulderWeapon!= NULL)
					CurrentLeftShoulderWeapon->Destroy();
				CurrentLeftShoulderWeapon = NULL;
			}
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			if (SpawnedWeapon != NULL)
			{
				CurrentRightShoulderWeapon = SpawnedWeapon;
				CurrentRightShoulderWeapon->AttachRootComponentTo(GetMesh(), "RightShoulderWeaponSock", EAttachLocation::SnapToTarget);
			}
			else if (NewWeapon != NULL)
			{
				if (CurrentRightShoulderWeapon != NULL)
					CurrentRightShoulderWeapon->Destroy();

				SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon->GetClass(), spawnParams);
				SpawnedWeapon->AttachRootComponentTo(GetMesh(), "RightShoulderWeaponSock", EAttachLocation::SnapToTarget);
				SpawnedWeapon->SetActorScale3D(FVector(0.5, 0.25, 0.5));

				SpawnedWeapon->CurrentSlotType = In;
				CurrentRightShoulderWeapon = SpawnedWeapon;
				if (NewWeapon->CurrentProjectile != NULL)
					CurrentRightShoulderWeapon->CurrentProjectile = NewWeapon->CurrentProjectile;
			}
			else
			{
				if(CurrentRightShoulderWeapon!=NULL)
					CurrentRightShoulderWeapon->Destroy();
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

void AGOLMCharacter::FireWeapon(EEquipSlot WeaponSlot, bool StartFiring)
{
	if(Role == ROLE_Authority)
	{
		switch (WeaponSlot)
		{
			case EEquipSlot::HAND_SLOT:
				if(CurrentHandWeapon != NULL)
					CurrentHandWeapon->bStartShooting = StartFiring;
				break;
			case EEquipSlot::LEFT_SHOULDER:
				if(CurrentLeftShoulderWeapon != NULL)
					CurrentLeftShoulderWeapon->bStartShooting = StartFiring;
				break;
			case EEquipSlot::RIGHT_SHOULDER:
				if(CurrentRightShoulderWeapon != NULL)
					CurrentRightShoulderWeapon->bStartShooting = StartFiring;
				break;
			default:break;
		}
	}
	if (Role != ROLE_Authority)
		ServerFireWeapon(WeaponSlot, StartFiring);
}
void AGOLMCharacter::ServerFireWeapon_Implementation(EEquipSlot WeaponSlot, bool StartFiring)
{
	FireWeapon(WeaponSlot, StartFiring);
}
bool AGOLMCharacter::ServerFireWeapon_Validate(EEquipSlot WeaponSlot, bool StartFiring)
{
	return true;
}


void AGOLMCharacter::MoveCheck()
{
	if (IsLocallyControlled())
	{
		if (!bMovingUp && !bMovingDown && !bMovingLeft && !bMovingRight)
		{
			bMoving = false;
		}
		else
			bMoving = true;
	
		FVector OverallDirection = FVector::ZeroVector;

		if(bMoving)
		{
				if (bMovingUp)		OverallDirection += PlayerCamera->GetForwardVector();
				if (bMovingDown)	OverallDirection += -PlayerCamera->GetForwardVector();
				if (bMovingRight)	OverallDirection += PlayerCamera->GetRightVector();
				if (bMovingLeft)	OverallDirection += -PlayerCamera->GetRightVector();
		}
			
		OverallDirection.Z = 0;
		OverallDirection.Normalize();

		FinalOrientation = OverallDirection.Rotation();

		if (Role != ROLE_Authority)
		{
			ServerMove(FinalOrientation,bMoving);
		}
	}
	
}

void AGOLMCharacter::ServerMove_Implementation(FRotator direction, bool bIsMoving)
{
	bMoving = bIsMoving;
	FinalOrientation = direction;
}
bool AGOLMCharacter::ServerMove_Validate(FRotator direction, bool bIsMoving){return true;}



void AGOLMCharacter::Boost()
{
	//if (Role != ROLE_Authority || IsLocallyControlled())
	if(bAlive)
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
	if (!UKismetSystemLibrary::IsValid(CurrentHandWeapon))
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

		RespawnTimeCheck = 0;
		Health = 100;

		ReturnHomeTimer = 0;

		if (!bIsAI)
			Cast<AGOLMGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->KillAllEnemies();

		//ChangePlayerCollisionProfile(NoPawnCollisionProfile);
		LoadEntranceLevel("LockerRoom");

		if (!bAlive)
		{

			bAlive = true;
			SetRagDoll(false);
		}
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
	if(CurrentLevelStream != "LockerRoom" && bCanGoHome)
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


	if (CurrentHandWeapon == NULL && CurrentLeftShoulderWeapon == NULL && CurrentRightShoulderWeapon == NULL)
	{
		NewCameraOffset = FVector::ZeroVector;
		return;
	}

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

	if (bIsAI)
	{
		if (Role == ROLE_Authority)
		{
			AGOLMGameMode *GM = Cast<AGOLMGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if(GM != NULL)
				GM->NumOfEnemies--;
		}
	}
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

		//UGameplayStatics::GetStreamingLevel(GetWorld(), EntranceLevelName)->bShouldBeVisible = true;
		//if (CurrentLevelStream != "None" && CurrentLevelStream != EntranceLevelName)
		//	UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentLevelStream)->bShouldBeVisible = false;

		CurrentLevelStream = EntranceLevelName;
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


void AGOLMCharacter::LoadEntranceLevel(AGOLMLevelStreamBeacon *LevelBeacon)
{
	if (IsLocallyControlled() && Role != ROLE_Authority)
	{
		if (LevelBeacon != NULL)
		{

			//UGameplayStatics::GetStreamingLevel(GetWorld(), LevelBeacon->NameOfLevelToLoad)->bShouldBeVisible = true;
			//if (CurrentLevelStream != "None" && CurrentLevelStream != LevelBeacon->NameOfLevelToLoad)
			//	UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentLevelStream)->bShouldBeVisible = false;

			CurrentLevelStream = LevelBeacon->NameOfLevelToLoad;
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "LevelBeacon is NULL");
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


void AGOLMCharacter::MoveToEntrance(FName EntranceLevelNameTag)
{
	if (Role == ROLE_Authority)
	{
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
					SetActorLocation(StartLocation->GetSpawnLocation());


					//TeleportTo(StartLocation->GetSpawnLocation(), GetActorRotation());
					CurrentLevelStream = EntranceLevelNameTag;
					if (CurrentLevelStream == "LockerRoom")
					{
						bStartHomeTimer = false;
						ReturnHomeTimer = 0;
						bCanGoHome = false;
					}
					else
					{
						bStartHomeTimer = true;
					}
					break;
				}
			}

		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "NO SPAWN LOCATIONS");


		if (EntranceLevelNameTag == "LockerRoom")
			SetPawnCollisionType(EPlayerCollisionProfile::LOCKER);
		else
			SetPawnCollisionType(EPlayerCollisionProfile::REGULAR);
	}
	
	if (Role != ROLE_Authority)
	{
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
					CurrentLevelStream = LevelBeacon->NameOfLevelToLoad;

					if (CurrentLevelStream == "LockerRoom")
					{
						bStartHomeTimer = false;
						ReturnHomeTimer = 0;
						bCanGoHome = false;
					}
					else
					{
						bStartHomeTimer = true;
					}
					break;
				}
			}
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "NO SPAWN LOCATIONS");
	}

	if (Role != ROLE_Authority)
	{
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

			}
			else
			{
				
				GetMesh()->SetCollisionProfileName("CharacterMesh");
				GetMesh()->AttachTo(GetCapsuleComponent(), NAME_None, EAttachLocation::SnapToTarget, true);
				//GetMesh()->AddLocalOffset(FVector(0, 0, 0));
				GetMesh()->AddLocalRotation(FRotator(0, -90, 0));
			}

			if (CurrentHandWeapon != NULL)
			{
				CurrentHandWeapon->SetRagDoll(value);
				if (value)
					CurrentHandWeapon->DetachRootComponentFromParent(true);

			}
			if (CurrentLeftShoulderWeapon != NULL)
			{
				CurrentLeftShoulderWeapon->SetRagDoll(value);
				if (value)
					CurrentLeftShoulderWeapon->DetachRootComponentFromParent(true);

			}
			if (CurrentRightShoulderWeapon != NULL)
			{
				CurrentRightShoulderWeapon->SetRagDoll(value);
				if (value)
					CurrentRightShoulderWeapon->DetachRootComponentFromParent(true);
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
	{
		if(bMoving)
		{
			if(UKismetSystemLibrary::IsValid(CurrentHandWeapon))
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
		}
		else
		{
			RelativeForward = RelativeRight = 0;
		}
	}
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

void AGOLMCharacter::ShowCompatibleProjectiles()
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
		if(!bGodMode)
		{
			Health -= Damage;
			if (Health <= 0)
				SetRagDoll(true);
		}

	}
	return Damage;
}

void AGOLMCharacter::OnRep_OnEquippedHandWeapon()
{
	//if(IsLocallyControlled())
	{ 
		if(UKismetSystemLibrary::IsValid(CurrentHandWeapon))
		{
			CurrentHandWeapon->WeaponMesh->AddLocalRotation(FRotator(0,180,-180));
			if(bIsInMenu)
				ShowCompatibleProjectiles();
		}
	}
}

void AGOLMCharacter::OnRep_OnEquippedRightShoulder()
{
	//if (IsLocallyControlled())
	{
		if (UKismetSystemLibrary::IsValid(CurrentRightShoulderWeapon))
		{
			if (bIsInMenu)
				ShowCompatibleProjectiles();
		}
	}
}

void AGOLMCharacter::OnRep_OnEquippedLeftShoulderWeapon()
{
	//if (IsLocallyControlled())
	{
		if (UKismetSystemLibrary::IsValid(CurrentLeftShoulderWeapon))
		{
			if (bIsInMenu)
				ShowCompatibleProjectiles();
		}
	}
}

void AGOLMCharacter::ChangeColor(USkeletalMesh *NewSkin)
{
	GetMesh()->SetSkeletalMesh(NewSkin);
	if (Role != ROLE_Authority)
	{
		
		ServerChangeColor(NewSkin);
	}
	if (Role == ROLE_Authority)
	{
		ClientChangeColor(NewSkin);
	}
}

void AGOLMCharacter::ServerChangeColor_Implementation(USkeletalMesh *NewSkin)
{
	ChangeColor(NewSkin);
}

bool AGOLMCharacter::ServerChangeColor_Validate(USkeletalMesh *NewSkin)
{
	return true;
}
void AGOLMCharacter::ClientChangeColor_Implementation(USkeletalMesh *NewSkin)
{
	if (Role != ROLE_Authority)
		GetMesh()->SetSkeletalMesh(NewSkin);
}