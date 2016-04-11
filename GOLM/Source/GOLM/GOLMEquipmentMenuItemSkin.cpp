// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMEquipmentMenuItemSkin.h"
#include "GOLMEquipmentMenuWidget.h"




void UGOLMEquipmentMenuItemSkin::SetSkinColor(USkeletalMesh *NewSkin)
{
	if (NewSkin != NULL)
		CurrentSkin = NewSkin;
}

void UGOLMEquipmentMenuItemSkin::GetSkin()
{
	AGOLMPlayerController *PlayerCon = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerCon->ChangeSkin(CurrentSkin);
}

FName UGOLMEquipmentMenuItemSkin::GetSkinName()
{
	return FName(*CurrentSkin->GetName());
}