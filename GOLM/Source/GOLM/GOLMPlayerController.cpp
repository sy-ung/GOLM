// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GOLM.h"
#include "GOLMPlayerController.h"
#include "GOLMGameMode.h"
#include "GOLMMouseWidget.h"
#include "GOLMEquipmentMenuWidget.h"
#include "GOLMInGameHUDWidget.h"
#include "Engine.h"
#include "AI/Navigation/NavigationSystem.h"

AGOLMPlayerController::AGOLMPlayerController()
{
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bShowMouseCursor = false;
}



void AGOLMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<AGOLMCharacter>(GetPawn());
	CursorWidgetReference = Cast<UGOLMMouseWidget>( CreateWidget<UUserWidget>(this, CursorWidget.GetDefaultObject()->GetClass()) );
	if (CursorWidgetReference != NULL)
	{
		CursorWidgetReference->AddToViewport(3);
		ChangeCursor(EPlayerCursorType::MENU);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "CURSOR WIDGET IS NULL");
	//bShowMouseCursor = true;
}

void AGOLMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(CursorWidgetReference != NULL)
		CursorWidgetReference->MoveMouseCursor(this);
}

void AGOLMPlayerController::FireHandWeapon(bool value)
{

	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
	if (!PlayerChar->bIsInMenu)
		PlayerChar->FireHandWeapon(value);
}
void AGOLMPlayerController::FireLeftShoulderWeapon(bool value)
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
	if (!PlayerChar->bIsInMenu)
		PlayerChar->FireLeftShoulderWeapon(value);
}
void AGOLMPlayerController::FireRightShoulderWeapon(bool value)
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
	if (!PlayerChar->bIsInMenu)
		PlayerChar->FireRightShoulderWeapon(value);
}


FVector AGOLMPlayerController::GetMouseHit()
{
	
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		
		FHitResult Hit(ForceInit);
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, Hit);
		return Hit.ImpactPoint;

	}
	
	return FVector::ZeroVector;
	
}

void AGOLMPlayerController::RotatePlayerCamera(bool value)
{
	
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->bRotatingCamera = value;
	}

}

void AGOLMPlayerController::MovePlayerUp(bool value)
{
	if(!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
			if (PlayerChar)	PlayerChar->bMovingUp = value;
		if (Role != ROLE_Authority)	ServerMovePlayerUp(value);
	}
}
void AGOLMPlayerController::ServerMovePlayerUp_Implementation(bool value){ MovePlayerUp(value); }
bool AGOLMPlayerController::ServerMovePlayerUp_Validate(bool value){ return true; }

void AGOLMPlayerController::MovePlayerDown(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)	PlayerChar->bMovingDown = value;
		if (Role != ROLE_Authority)	ServerMovePlayerDown(value);
	}
}
void AGOLMPlayerController::ServerMovePlayerDown_Implementation(bool value) { MovePlayerDown(value); }
bool AGOLMPlayerController::ServerMovePlayerDown_Validate(bool value) { return true; }


void AGOLMPlayerController::MovePlayerLeft(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar) PlayerChar->bMovingLeft = value;
		if (Role != ROLE_Authority)	ServerMovePlayerLeft(value);
	}
}
void AGOLMPlayerController::ServerMovePlayerLeft_Implementation(bool value) { MovePlayerLeft(value); }
bool AGOLMPlayerController::ServerMovePlayerLeft_Validate(bool value) { return true; }


void AGOLMPlayerController::MovePlayerRight(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)	PlayerChar->bMovingRight = value;
		if (Role != ROLE_Authority)	ServerMovePlayerRight(value);
	}
}
void AGOLMPlayerController::ServerMovePlayerRight_Implementation(bool value) { MovePlayerRight(value); }
bool AGOLMPlayerController::ServerMovePlayerRight_Validate(bool value) { return true; }


void AGOLMPlayerController::BoostPlayer(bool value)
{
	if (Role == ROLE_Authority)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->Boost();
			//PlayerChar->bBoosting = value;
	}
	else if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		ServerBoostPlayer(value);
	}
}
void AGOLMPlayerController::ServerBoostPlayer_Implementation(bool value)
{
	BoostPlayer(value);
}
bool AGOLMPlayerController::ServerBoostPlayer_Validate(bool value)
{
	return true;
}


void AGOLMPlayerController::ZoomPlayerCamera(float deltaZoom)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->ZoomCamera(deltaZoom);
	}
}

void AGOLMPlayerController::GotoLockerRoom()
{
	if(IsLocalController())
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

void AGOLMPlayerController::ChangeWeapon(AWeapon *NewWeapon, EEquipSlot Slot)
{
	//if (CheckIsNotServer())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		AGOLMPlayerState *PS = Cast<AGOLMPlayerState>(PlayerState);
		if (PlayerChar)
		{
			if (PlayerChar->CurrentLevelStream == "LockerRoom")
			{
				PlayerChar->Equip(NewWeapon, Slot);
				EquipmentMenuReference->SetupWeaponProjectileSelection();
			}
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

void AGOLMPlayerController::ChangeCursor(EPlayerCursorType NewCursor)
{
	UImage *MouseImage = Cast<UGOLMMouseWidget>(CursorWidgetReference)->GetMouseReference();
	if (MouseImage != NULL)
	{
		switch (NewCursor)
		{
		case EPlayerCursorType::CROSSHAIR:
			MouseImage->SetBrushFromTexture(Crosshair);
			break;
		case EPlayerCursorType::MENU:
			MouseImage->SetBrushFromTexture(MenuCursor);
		default:break;
		}

	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "MOUSE IS NULL");
	CurrentCursorType = NewCursor;
}


void AGOLMPlayerController::MenuSetWeaponSlotChoice(EEquipSlot Choice)
{
	if(IsLocalController())
	{
		MenuSlotChoice = Choice;
		switch (Choice)
		{
		case EEquipSlot::HAND_SLOT:
			GotoFrontCamera();
			break;
		case EEquipSlot::LEFT_SHOULDER:
			GotoLeftShoulderCamera();
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			GotoRightShoulderCamera();
			break;
		default:
			break;
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "I AM BEING CALLED");
		EquipmentMenuReference->SetupWeaponSelection();
	}
}

EEquipSlot AGOLMPlayerController::GetMenuWeaponSlotChoice()
{
	return MenuSlotChoice;
}

void AGOLMPlayerController::ShowEquipmentMenu()
{
	if(IsLocalController())
	{
		if(!bIsInEquipmentMenu && !bIsInSettingsMenu)
		{
			if(Cast<AGOLMCharacter>(GetCharacter())->CurrentLevelStream == "LockerRoom")
			{
				if (EquipmentMenuReference == NULL)
				{
					EquipmentMenuReference = Cast< UGOLMEquipmentMenuWidget>(CreateWidget<UUserWidget>(this, EquipmentMenu.GetDefaultObject()->GetClass()) );
					EquipmentMenuReference->AddToRoot();
		
				}

				if (EquipmentMenuReference != NULL)
				{
				
					
					FInputModeGameAndUI UI;

					SetInputMode(UI);
					ChangeCursor(EPlayerCursorType::MENU);
					EquipmentMenuReference->AddToViewport(1);
					UI.SetWidgetToFocus(EquipmentMenuReference->GetCachedWidget());
					bIsInEquipmentMenu = true;
					Cast<AGOLMCharacter>(GetCharacter())->bIsInMenu = true;
					
				}
				else
					GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "EquipmentMenuReference is NULL");

				
			}
		}
	}
}
void AGOLMPlayerController::HideEquipmentMenu()
{
	if(IsLocalController())
	{
		if(bIsInEquipmentMenu && !bIsInSettingsMenu)
		{
			EquipmentMenuReference->RemoveFromParent();
			FInputModeGameAndUI GI;
			GI.SetHideCursorDuringCapture(false);
			SetInputMode(GI);
			ChangeCursor(EPlayerCursorType::CROSSHAIR);
			bIsInEquipmentMenu = false;
			Cast<AGOLMCharacter>(GetCharacter())->bIsInMenu = false;
			GotoPlayerCamera();
		}
	}
}

void AGOLMPlayerController::ShowInGameSettingsMenu()
{
	if (IsLocalController())
	{
		if(!bIsInSettingsMenu)
		{
			if (InGameSettingsMenuReference == NULL)
			{
				InGameSettingsMenuReference = CreateWidget<UUserWidget>(this, InGameSettingsMenu.GetDefaultObject()->GetClass());
				InGameSettingsMenuReference->AddToRoot();
			}

			if (InGameSettingsMenuReference != NULL)
			{
				FInputModeGameAndUI UI;
				UI.SetWidgetToFocus(InGameSettingsMenuReference->GetCachedWidget());
				SetInputMode(UI);
				ChangeCursor(EPlayerCursorType::MENU);
				InGameSettingsMenuReference->AddToViewport(2);
				bIsInSettingsMenu = true;
			}
		}
	}
}

void AGOLMPlayerController::HideInGameSettingsMenu()
{
	if(bIsInSettingsMenu)
	{
		FInputModeGameAndUI GI;
		GI.SetHideCursorDuringCapture(false);
		SetInputMode(GI);
		ChangeCursor(EPlayerCursorType::CROSSHAIR);
		InGameSettingsMenuReference->RemoveFromParent();
		bIsInSettingsMenu = false;
	}
}

void AGOLMPlayerController::ShowInGameHud()
{
	if (IsLocalController())
	{
		if (InGameHUDReference == NULL)
		{
			InGameHUDReference = Cast<UGOLMInGameHudWidget>( CreateWidget<UUserWidget>(this, InGameHUD.GetDefaultObject()->GetClass()) );
			InGameHUDReference->AddToRoot();
		}

		if (InGameHUDReference != NULL)
		{
			InGameHUDReference->AddToViewport();
		}
	}
}

void AGOLMPlayerController::HideInGameHud()
{
	if (InGameHUDReference != NULL)
		InGameHUDReference->RemoveFromParent();
}
void AGOLMPlayerController::ZoomMiniMap(float value)
{
	if (InGameHUDReference != NULL)
	{
		Cast<AGOLMCharacter>(GetCharacter())->ZoomMiniMapCamera(value);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "IN GAME HUD IS NULL");
}

void AGOLMPlayerController::GotoPlayerCamera()
{
	SetViewTargetWithBlend(Cast<AGOLMCharacter>(GetCharacter())->PlayerCameraActor, 0.5f);
	EquipmentMenuReference->SetupWeaponProjectileSelection();
	
}
void AGOLMPlayerController::GotoFrontCamera()
{
	SetViewTargetWithBlend(Cast<AGOLMCharacter>(GetCharacter())->FrontCameraActor, 0.5f);
	EquipmentMenuReference->SetupWeaponProjectileSelection();
}

void AGOLMPlayerController::GotoLeftShoulderCamera()
{
	SetViewTargetWithBlend(Cast<AGOLMCharacter>(GetCharacter())->LeftShoulderCameraActor, 0.5f);
	EquipmentMenuReference->SetupWeaponProjectileSelection();
}
void AGOLMPlayerController::GotoRightShoulderCamera()
{
	SetViewTargetWithBlend(Cast<AGOLMCharacter>(GetCharacter())->RightShoulderCameraActor, 0.5f);
	EquipmentMenuReference->SetupWeaponProjectileSelection();
}

void AGOLMPlayerController::SetArcFire(bool value)
{
	AGOLMCharacter *ConChar = Cast<AGOLMCharacter>(GetCharacter());
	if (ConChar != NULL)
	{
		if(ConChar->CurrentHandWeapon != NULL)
			ConChar->CurrentHandWeapon->bArcFire = value;

		if (ConChar->CurrentLeftShoulderWeapon != NULL)
			ConChar->CurrentLeftShoulderWeapon->bArcFire = value;

		bIsArcFireOn = value;
	}
}