// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMEquipmentMenuItemWeapon.h"




void UGOLMEquipmentMenuItemWeapon::SetWeapon(AWeapon *SavedWeapon)
{
	Weapon = SavedWeapon;
}
void UGOLMEquipmentMenuItemWeapon::GetWeapon()
{	
	AGOLMPlayerController *PlayerCon = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerCon->ChangeWeapon(Weapon, PlayerCon->GetMenuWeaponSlotChoice());
}

FName UGOLMEquipmentMenuItemWeapon::GetWeaponName()
{
	return Weapon->Name;
}