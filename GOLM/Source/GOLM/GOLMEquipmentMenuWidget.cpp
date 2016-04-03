// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMEquipmentMenuWidget.h"


void UGOLMEquipmentMenuWidget::SetupWeaponSelection()
{

	WeaponScrollBox->ClearChildren();

	if(PlayerCon == NULL)
		PlayerCon = Cast<AGOLMPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<TSubclassOf<AWeapon>> WeaponList = Cast<AGOLMPlayerState>(PlayerCon->PlayerState)->WeaponsList;
		
	for (int32 i = 0; i < WeaponList.Num(); i++)
	{

		if (WeaponList[i] == NULL)
		{
			AddToWeaponSelection(WeaponList[i]);
		}
		else if(WeaponList[i].GetDefaultObject()->CompatibleWeaponSlotType == EEquipSlot::ALL)
		{
			AddToWeaponSelection(WeaponList[i]);
		}
		else if(WeaponList[i].GetDefaultObject()->CompatibleWeaponSlotType == PlayerCon->GetMenuWeaponSlotChoice())
		{
			AddToWeaponSelection(WeaponList[i]);
		}
		else if (WeaponList[i].GetDefaultObject()->CompatibleWeaponSlotType == EEquipSlot::LEFT_RIGHT)
		{
			if (PlayerCon->GetMenuWeaponSlotChoice() == EEquipSlot::LEFT_SHOULDER ||
				PlayerCon->GetMenuWeaponSlotChoice() == EEquipSlot::RIGHT_SHOULDER)
			{
				AddToWeaponSelection(WeaponList[i]);
			}
		}
		else if (WeaponList[i].GetDefaultObject()->CompatibleWeaponSlotType == EEquipSlot::HAND_LEFT)
		{
			if (PlayerCon->GetMenuWeaponSlotChoice() == EEquipSlot::HAND_SLOT ||
				PlayerCon->GetMenuWeaponSlotChoice() == EEquipSlot::LEFT_SHOULDER)
			{
				AddToWeaponSelection(WeaponList[i]);
			}
		}
		else if (WeaponList[i].GetDefaultObject()->CompatibleWeaponSlotType == EEquipSlot::HAND_RIGHT)
		{
			if (PlayerCon->GetMenuWeaponSlotChoice() == EEquipSlot::HAND_SLOT ||
				PlayerCon->GetMenuWeaponSlotChoice() == EEquipSlot::RIGHT_SHOULDER)
			{
				AddToWeaponSelection(WeaponList[i]);
			}
		}
	}
}
void UGOLMEquipmentMenuWidget::AddToWeaponSelection(TSubclassOf<AWeapon> NewWeapon)
{
	UGOLMEquipmentMenuItemWeapon *NewWeaponItem = CreateWidget<UGOLMEquipmentMenuItemWeapon>(PlayerCon, WeaponItemWidget.GetDefaultObject()->GetClass());
	NewWeaponItem->SetWeapon(NewWeapon.GetDefaultObject());
	WeaponScrollBox->AddChild(NewWeaponItem);
}


void UGOLMEquipmentMenuWidget::SetupWeaponProjectileSelection()
{
	ProjectileScrollBox->ClearChildren();
	if (PlayerCon == NULL)
		PlayerCon = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AWeapon *SelectedWeapon = Cast<AWeapon>(Cast<AGOLMCharacter>(PlayerCon->GetCharacter())->CurrentHandWeapon);

	if(SelectedWeapon != NULL)
	{
		TArray<TSubclassOf<AProjectile>> ProjectileList = SelectedWeapon->CompatibleProjectiles;

		for (int32 i = 0; i < ProjectileList.Num(); i++)
		{
			UGOLMEquipmentMenuItemProjectile *NewProjectileItem = CreateWidget<UGOLMEquipmentMenuItemProjectile>(PlayerCon, ProjectileItemWidget.GetDefaultObject()->GetClass());
			NewProjectileItem->SetProjectile(ProjectileList[i].GetDefaultObject());
			ProjectileScrollBox->AddChild(NewProjectileItem);
		}
	}
}

void UGOLMEquipmentMenuWidget::SetWeaponScrollBox(UScrollBox *WeaponListScollBox)
{
	WeaponScrollBox = WeaponListScollBox;
}
void UGOLMEquipmentMenuWidget::SetProjectileScrollBox(UScrollBox *ProjectileListScollBox)
{
	ProjectileScrollBox = ProjectileListScollBox;
}



