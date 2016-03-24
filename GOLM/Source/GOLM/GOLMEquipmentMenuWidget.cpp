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
		UGOLMEquipmentMenuItemWeapon *NewWeaponItem = CreateWidget<UGOLMEquipmentMenuItemWeapon>(PlayerCon, WeaponItemWidget.GetDefaultObject()->GetClass());
		NewWeaponItem->SetWeapon(WeaponList[i].GetDefaultObject());
		WeaponScrollBox->AddChild(NewWeaponItem);
	}
}

void UGOLMEquipmentMenuWidget::SetupWeaponProjectileSelection()
{
	ProjectileScrollBox->ClearChildren();
	if (PlayerCon == NULL)
		PlayerCon = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AWeapon *SelectedWeapon = Cast<AWeapon>(Cast<AGOLMCharacter>(PlayerCon->GetCharacter())->CurrentWeapon);

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



