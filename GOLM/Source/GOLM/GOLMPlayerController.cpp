// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GOLM.h"
#include "GOLMPlayerController.h"
#include "GOLMGameMode.h"
#include "Engine.h"
#include "AI/Navigation/NavigationSystem.h"

AGOLMPlayerController::AGOLMPlayerController()
{
	
	//DefaultMouseCursor = EMouseCursor::Crosshairs;
	
}



void AGOLMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<AGOLMCharacter>(GetPawn());
	//bShowMouseCursor = true;
}

void AGOLMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AGOLMPlayerController::FireWeapon(bool value)
{
	//if(CheckIsNotServer())
	//{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bStartShooting = value;
	//}
}

void AGOLMPlayerController::Aim()
{

}


FVector AGOLMPlayerController::GetMouseHit()
{
	if(CheckIsNotServer())
	{
		FHitResult Hit(ForceInit);
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, Hit);
		return Hit.ImpactPoint;
	}
	else
	{
		return FVector::ZeroVector;
	}
}

void AGOLMPlayerController::RotatePlayerCamera(bool value)
{
	if(CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bRotatingCamera = value;
		else
		{

		}
	}

}
void AGOLMPlayerController::MovePlayerCamera(bool value)
{
	if(CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bMovingCamera = value;
	}
}

void AGOLMPlayerController::MovePlayerUp(bool value)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bMovingUp = value;
	}
}
void AGOLMPlayerController::MovePlayerDown(bool value)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bMovingDown = value;
	}
}
void AGOLMPlayerController::MovePlayerLeft(bool value)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bMovingLeft = value;
	}
}
void AGOLMPlayerController::MovePlayerRight(bool value)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bMovingRight = value;
	}
}
void AGOLMPlayerController::BoostPlayer(bool value)
{
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bBoosting = value;
	}
}



void AGOLMPlayerController::ZoomPlayerCamera(float deltaZoom)
{
	if(CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->ZoomCamera(deltaZoom);
	}
}
void AGOLMPlayerController::Boost(bool value)
{
	if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bBoosting = value;
	}
}

void AGOLMPlayerController::GotoLockerRoom()
{
	if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->GotoLockerRoom();
	}
}
void AGOLMPlayerController::ServerGotoLockerRoom_Implementation()
{

}
bool AGOLMPlayerController::ServerGotoLockerRoom_Validate()
{
	return true;
}
void AGOLMPlayerController::RespawnCharacter()
{
	if (Role == ROLE_Authority)
	{
		Cast<AGOLMCharacter>(GetCharacter())->Respawn();
	}

}
void AGOLMPlayerController::ServerRespawn_Implementation()
{
	RespawnCharacter();
}
bool AGOLMPlayerController::ServerRespawn_Validate()
{
	return true;
}

void AGOLMPlayerController::ChangeWeapon(EGetWeapon NewWeapon, EEquipSlot Slot)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		AGOLMPlayerState *PS = Cast<AGOLMPlayerState>(PlayerState);
		if (PlayerChar)
		{
			if (PlayerChar->CurrentLevelStream == "LockerRoom")
				PlayerChar->Equip(NewWeapon, Slot);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Character is NULL from Controller");
			return;
		}
		if (PS)
			PS->SetWeaponFor(NewWeapon, Slot);
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Player State is NULL from Controller");
			return;
		}

		
	}
}

void AGOLMPlayerController::GetEquippedWeapons()
{
	if (Role != ROLE_Authority || IsLocalController())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->GetEquippedWeapons();
	}
	else
		ClientGetEquippedWeapons();
}

void AGOLMPlayerController::ClientGetEquippedWeapons_Implementation()
{
	if(Role!=ROLE_Authority)
		GetEquippedWeapons();
}

void AGOLMPlayerController::DoDamage(float damage)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->RecieveDamage(damage);
	}
}

void AGOLMPlayerController::KillCharacter()
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->Death();
	}
}


bool AGOLMPlayerController::CheckIsNotServer()
{

	return GetCharacter()->IsLocallyControlled();
		

	if (Role != ROLE_Authority)
		return true;
	else
		return false;

}