// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMEquipmentMenuWidget.h"


void UGOLMEquipmentMenuWidget::SetupWeaponSelection()
{
	if(!bListCreated)
	{
		PlayerCon = Cast<AGOLMPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
		TArray<TSubclassOf<AWeapon>> WeaponList = Cast<AGOLMPlayerState>(PlayerCon->PlayerState)->WeaponsList;
	
		for (int32 i = 0; i < WeaponList.Num(); i++)
		{
			UGOLMEquipmentMenuItemWeapon *NewWeaponItem = CreateWidget<UGOLMEquipmentMenuItemWeapon>(PlayerCon, WeaponItemWidget.GetDefaultObject()->GetClass());
			NewWeaponItem->SetWeapon(WeaponList[i].GetDefaultObject());
			WeaponScrollBox->AddChild(NewWeaponItem);
			WeaponListItems.Add(NewWeaponItem);
		}
		bListCreated = true;
	}

}

void UGOLMEquipmentMenuWidget::SetScrollBox(UScrollBox *WeaponListScollBox)
{
	WeaponScrollBox = WeaponListScollBox;
}



