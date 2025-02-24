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
	AWeapon *SelectedWeapon = NULL;

	if(PlayerCon != NULL)
	{
		switch (PlayerCon->GetMenuWeaponSlotChoice())
		{
		case EEquipSlot::HAND_SLOT:
			SelectedWeapon = Cast<AWeapon>(Cast<AGOLMCharacter>(PlayerCon->GetCharacter())->CurrentHandWeapon);
			break;
		case EEquipSlot::LEFT_SHOULDER:
			SelectedWeapon = Cast<AWeapon>(Cast<AGOLMCharacter>(PlayerCon->GetCharacter())->CurrentLeftShoulderWeapon);
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			SelectedWeapon = Cast<AWeapon>(Cast<AGOLMCharacter>(PlayerCon->GetCharacter())->CurrentRightShoulderWeapon);
			break;
		default:break;
		}
	

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
}

void UGOLMEquipmentMenuWidget::SetupSkinSelection()
{
	SkinScrollBox->ClearChildren();

	if (PlayerCon == NULL)
		PlayerCon = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<USkeletalMesh*> Skins = Cast<AGOLMPlayerState>(PlayerCon->PlayerState)->SkinList;

	for (int32 i = 0; i < Skins.Num(); i++)
	{
		UGOLMEquipmentMenuItemSkin *NewSkinItem = CreateWidget<UGOLMEquipmentMenuItemSkin>(PlayerCon, SkinItemWidget.GetDefaultObject()->GetClass());
		NewSkinItem->SetSkinColor(Skins[i]);
		SkinScrollBox->AddChild(NewSkinItem);
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
void UGOLMEquipmentMenuWidget::SetSkinScrollBox(UScrollBox *SkinListScollBox)
{
	SkinScrollBox = SkinListScollBox;
}


