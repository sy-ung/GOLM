// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMEquipmentMenuItemProjectile.h"




void UGOLMEquipmentMenuItemProjectile::SetProjectile(AProjectile *NewProjectile)
{
	CurrentProjectile = NewProjectile;
}

void UGOLMEquipmentMenuItemProjectile::GetProjectile()
{
	
	Cast<AGOLMCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter())->CurrentWeapon->SetNewProjectile(CurrentProjectile);
}

FName UGOLMEquipmentMenuItemProjectile::GetProjectileName()
{
	if (CurrentProjectile != NULL)
		return CurrentProjectile->Name;
	else
		return "None";
}