// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMEquipmentMenuItemWeapon.h"
#include "GOLMEquipmentMenuWidget.h"




void UGOLMEquipmentMenuItemWeapon::SetWeapon(AWeapon *SavedWeapon)
{
	if(SavedWeapon!=NULL)
		Weapon = SavedWeapon;
}
void UGOLMEquipmentMenuItemWeapon::GetWeapon()
{	
	AGOLMPlayerController *PlayerCon = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerCon->ChangeWeapon(Weapon, PlayerCon->GetMenuWeaponSlotChoice());
}

FName UGOLMEquipmentMenuItemWeapon::GetWeaponName()
{
	if (Weapon != NULL)
		return Weapon->Name;
	else
		return "None";
}