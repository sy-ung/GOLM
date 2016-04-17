// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GOLM.h"
#include "GOLMPlayerController.h"
#include "GOLMGameMode.h"
#include "GOLMMouseWidget.h"
#include "GOLMEquipmentMenuWidget.h"
#include "GOLMInGameHUDWidget.h"
#include "GOLMGameInstance.h"
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

void AGOLMPlayerController::SetCharacterName(FName NewName)
{
	if(Role == ROLE_Authority)
		Cast<AGOLMCharacter>(GetCharacter())->CharacterName = NewName;
	if (Role != ROLE_Authority)
		ServerSetCharacterName(NewName);
}

void AGOLMPlayerController::ServerSetCharacterName_Implementation(FName NewName)
{
	SetCharacterName(NewName);
}
bool AGOLMPlayerController::ServerSetCharacterName_Validate(FName NewName)
{
	return true;
}

void AGOLMPlayerController::GetCharacterName()
{
	if (Role != ROLE_Authority || IsLocalController())
	{
		SetCharacterName(Cast<UGOLMGameInstance>(GetGameInstance())->CurrentPlayerName);
	}

	if (Role == ROLE_Authority)
		ClientGetCharacterName();
}
void AGOLMPlayerController::ClientGetCharacterName_Implementation()
{
	if (Role != ROLE_Authority)
		GetCharacterName();
}

void AGOLMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(CursorWidgetReference != NULL)
		CursorWidgetReference->MoveMouseCursor(this);
	if (InGameHUDReference != NULL)
	{
		InGameHUDReference->UpdateWeaponBar();
		InGameHUDReference->UpdateReturnHomeCooldownUI();
	}
}


void AGOLMPlayerController::FireWeapon(EEquipSlot WeaponSlot, bool StartShooting, bool InstantFire)
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());
	if (!PlayerChar->bIsInMenu)
		PlayerChar->FireWeapon(InGameHUDReference->GetCurrentWeaponSelection(), StartShooting);

	if (InstantFire)
		PlayerChar->FireWeapon(WeaponSlot, StartShooting);

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

FVector AGOLMPlayerController::GetMouseMovement()
{

	FVector MouseWorldLocation = FVector::ZeroVector;
	FVector MouseWorldDirection;
	
	DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	
	return MouseWorldDirection;
	

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
			//if (Role != ROLE_Authority)	ServerMovePlayerUp(value);
	}
}


void AGOLMPlayerController::MovePlayerDown(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)	PlayerChar->bMovingDown = value;

	}
}


void AGOLMPlayerController::MovePlayerLeft(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar) PlayerChar->bMovingLeft = value;
	
	}
}
void AGOLMPlayerController::MovePlayerRight(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)	PlayerChar->bMovingRight = value;
	}
}
void AGOLMPlayerController::BoostPlayer(bool value)
{
	if (!bIsInEquipmentMenu && !bIsInSettingsMenu)
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->Boost();
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

void AGOLMPlayerController::GotoLevel(FName LevelName)
{
	if (IsLocalController())
	{
		AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
		if (PlayerChar)
			PlayerChar->LoadEntranceLevel(LevelName);
	}
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
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetPawn());
	if (PlayerChar)
	{
		if (PlayerChar->CurrentLevelStream == "LockerRoom")
		{
			PlayerChar->Equip(NewWeapon, Slot);
			EquipmentMenuReference->SetupWeaponProjectileSelection();
			InGameHUDReference->SetWeaponBarSlot(Slot, NewWeapon);
			InGameHUDReference->SelectCurrentWeapon(Slot);
		}
	}
}

void AGOLMPlayerController::UpdateWeaponSelection(AWeapon *NewWeapon, EEquipSlot Slot)
{
	AGOLMPlayerState *PS = Cast<AGOLMPlayerState>(PlayerState);
	if (PS)
		PS->SetWeaponFor(NewWeapon, Slot);
		
}

void AGOLMPlayerController::ChangeSkin(USkeletalMesh *NewSkin)
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());
	PlayerChar->ChangeColor(NewSkin);
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
	if (CursorWidgetReference == NULL)
		return;

	if(NewCursor != CurrentCursorType)
	{
		Cast<UGOLMMouseWidget>(CursorWidgetReference)->ChangeMouseType(NewCursor);
		CurrentCursorType = NewCursor;
	}
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
					EquipmentMenuReference->SetupSkinSelection();
					
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

			AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());
			
			if(PlayerChar->CurrentHandWeapon == NULL && 
				PlayerChar->CurrentLeftShoulderWeapon == NULL && 
				PlayerChar->CurrentRightShoulderWeapon == NULL)
				ChangeCursor(EPlayerCursorType::MENU);
			else
				ChangeCursor(EPlayerCursorType::CROSSHAIR);

			UpdateWeaponSelection(PlayerChar->CurrentHandWeapon, EEquipSlot::HAND_SLOT);
			UpdateWeaponSelection(PlayerChar->CurrentLeftShoulderWeapon, EEquipSlot::LEFT_SHOULDER);
			UpdateWeaponSelection(PlayerChar->CurrentRightShoulderWeapon, EEquipSlot::RIGHT_SHOULDER);
		

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
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());

	if (PlayerChar)
	{
		PlayerChar->EquipmentCameraFrontBoom->bInheritYaw = false;
		PlayerChar->EquipmentCameraLeftShoulderBoom->bInheritYaw = false;
		PlayerChar->EquipmentCameraRightShoulderBoom->bInheritYaw = false;

		SetViewTargetWithBlend(PlayerChar->PlayerCameraActor, 0.5f);
		EquipmentMenuReference->SetupWeaponProjectileSelection();
	}
	
}
void AGOLMPlayerController::GotoFrontCamera()
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());

	if (PlayerChar)
	{
		PlayerChar->EquipmentCameraFrontBoom->bInheritYaw = true;
		SetViewTargetWithBlend(PlayerChar->FrontCameraActor, 0.5f);
		EquipmentMenuReference->SetupWeaponProjectileSelection();
	}


}

void AGOLMPlayerController::GotoLeftShoulderCamera()
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());

	if (PlayerChar)
	{
		PlayerChar->EquipmentCameraLeftShoulderBoom->bInheritYaw = true;
		SetViewTargetWithBlend(PlayerChar->LeftShoulderCameraActor, 0.5f);
		EquipmentMenuReference->SetupWeaponProjectileSelection();
	}
}
void AGOLMPlayerController::GotoRightShoulderCamera()
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(GetCharacter());

	if (PlayerChar)
	{
		PlayerChar->EquipmentCameraRightShoulderBoom->bInheritYaw = true;
		SetViewTargetWithBlend(PlayerChar->RightShoulderCameraActor, 0.5f);
		EquipmentMenuReference->SetupWeaponProjectileSelection();
	}
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

void AGOLMPlayerController::ReloadWeapon(EEquipSlot WeaponSlot)
{

	AGOLMCharacter *ConChar = Cast<AGOLMCharacter>(GetCharacter());
	if (ConChar != NULL)
	{
		switch (InGameHUDReference->GetCurrentWeaponSelection())
		{
		case EEquipSlot::HAND_SLOT:
			ConChar->CurrentHandWeapon->Reload();
			break;
		case EEquipSlot::LEFT_SHOULDER:
			ConChar->CurrentLeftShoulderWeapon->Reload();
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			ConChar->CurrentRightShoulderWeapon->Reload();
			break;
		default:
			break;
		}
	}
}

void AGOLMPlayerController::SelectWeapon(EEquipSlot WeaponSlot, bool GotoNext, bool GotoPrevious)
{
	if (InGameHUDReference != NULL)
	{
		if(GotoNext == false && GotoPrevious == false)
			InGameHUDReference->SelectCurrentWeapon(WeaponSlot);

		switch (InGameHUDReference->GetCurrentWeaponSelection())
		{
		case EEquipSlot::HAND_SLOT:
			if (GotoNext)
				InGameHUDReference->SelectCurrentWeapon(EEquipSlot::RIGHT_SHOULDER);
			else if (GotoPrevious)
				InGameHUDReference->SelectCurrentWeapon(EEquipSlot::LEFT_SHOULDER);

			Cast<AGOLMCharacter>(GetCharacter())->FireWeapon(EEquipSlot::HAND_SLOT, false);

			break;
		case EEquipSlot::LEFT_SHOULDER:
			if (GotoNext)
				InGameHUDReference->SelectCurrentWeapon(EEquipSlot::HAND_SLOT);
			else if (GotoPrevious)
				InGameHUDReference->SelectCurrentWeapon(EEquipSlot::RIGHT_SHOULDER);

			Cast<AGOLMCharacter>(GetCharacter())->FireWeapon(EEquipSlot::LEFT_SHOULDER, false);
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			if (GotoNext)
				InGameHUDReference->SelectCurrentWeapon(EEquipSlot::LEFT_SHOULDER);
			else if (GotoPrevious)
				InGameHUDReference->SelectCurrentWeapon(EEquipSlot::HAND_SLOT);

			Cast<AGOLMCharacter>(GetCharacter())->FireWeapon(EEquipSlot::RIGHT_SHOULDER, false);
			break;
		default:
			break;
		}
	}
}