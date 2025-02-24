// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMPlayerController.h"
#include "GOLMPlayerState.h"
#include "GOLMEquipmentMenuItemProjectile.h"




void UGOLMEquipmentMenuItemProjectile::SetProjectile(AProjectile *NewProjectile)
{
	CurrentProjectile = NewProjectile;
}

void UGOLMEquipmentMenuItemProjectile::GetProjectile()
{
	AGOLMCharacter *Player = Cast<AGOLMCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	AGOLMPlayerState *PS = Cast<AGOLMPlayerState>(Player->PlayerState);
	EEquipSlot CurrentSlotSelection = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetMenuWeaponSlotChoice();
	PS->GetWeaponFor(CurrentSlotSelection)->SetNewProjectile(CurrentProjectile);

	switch (CurrentSlotSelection)
	{
	case EEquipSlot::HAND_SLOT:
		Player->CurrentHandWeapon->SetNewProjectile(CurrentProjectile);
		break;
	case EEquipSlot::LEFT_SHOULDER:
		Player->CurrentLeftShoulderWeapon->SetNewProjectile(CurrentProjectile);
		break;
	case EEquipSlot::RIGHT_SHOULDER:
		Player->CurrentRightShoulderWeapon->SetNewProjectile(CurrentProjectile);
		break;
	default:
		break;
	}

	
	
}

FName UGOLMEquipmentMenuItemProjectile::GetProjectileName()
{
	if (CurrentProjectile != NULL)
		return CurrentProjectile->Name;
	else
		return "None";
}
UTexture2D *UGOLMEquipmentMenuItemProjectile::GetProjectileImage()
{
	if (CurrentProjectile != NULL)
		return CurrentProjectile->Image;
	else
		return NULL;
}