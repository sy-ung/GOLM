// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMInGameHudWidget.h"

void UGOLMInGameHudWidget::MapZoom(float value)
{
	FVector2D OriginalSize = MapCanvas->GetSize();
	FVector2D NewSize = FVector2D(OriginalSize.X + value, OriginalSize.Y + value);
	MapCanvas->SetSize(NewSize);
	MapCanvas->SetPosition(FVector2D(NewSize.X / -2, NewSize.Y / -2));
}

void UGOLMInGameHudWidget::UpdateMap()
{

}

void UGOLMInGameHudWidget::UpdateWeaponBar()
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (PlayerChar != NULL)
	{
		if(PlayerChar->CurrentHandWeapon != NULL)
		{
			if (bHandWeaponReloading)
			{
				PlayReload(EEquipSlot::HAND_SLOT, PlayerChar->CurrentHandWeapon);
				bHandWeaponReloading = PlayerChar->CurrentHandWeapon->bReloading;
				if (!bHandWeaponReloading)
				{
					SetWeaponBarSlot(EEquipSlot::HAND_SLOT, PlayerChar->CurrentHandWeapon);
				}
			}

			if(!bHandWeaponReloading)
			{
				HandWeaponAmmoCount->SetText(FText::FromString(
					FString::FromInt(PlayerChar->CurrentHandWeapon->CurrentAmmoCount) + "/" +
					FString::FromInt(PlayerChar->CurrentHandWeapon->MagazineSize)));
				HandWeaponTotalAmmoCount->SetText(FText::FromString(FString::FromInt(PlayerChar->CurrentHandWeapon->TotalAmmoCount)));
				bHandWeaponReloading = PlayerChar->CurrentHandWeapon->bReloading;
			}
		}

		if(PlayerChar->CurrentLeftShoulderWeapon != NULL)
		{
			if (bLeftShoulderWeaponReloading)
			{
				PlayReload(EEquipSlot::LEFT_SHOULDER, PlayerChar->CurrentLeftShoulderWeapon);
				bLeftShoulderWeaponReloading = PlayerChar->CurrentLeftShoulderWeapon->bReloading;
				if (!bLeftShoulderWeaponReloading)
				{
					SetWeaponBarSlot(EEquipSlot::LEFT_SHOULDER, PlayerChar->CurrentLeftShoulderWeapon);
				}
			}

			if(!bLeftShoulderWeaponReloading)
			{

				LeftShoulderWeaponWeaponAmmoCount->SetText(FText::FromString(
					FString::FromInt(PlayerChar->CurrentLeftShoulderWeapon->CurrentAmmoCount) + "/" +
					FString::FromInt(PlayerChar->CurrentLeftShoulderWeapon->MagazineSize)));
				LeftShoulderWeaponWeaponTotalAmmoCount->SetText(FText::FromString(FString::FromInt(PlayerChar->CurrentLeftShoulderWeapon->TotalAmmoCount)));
				bLeftShoulderWeaponReloading = PlayerChar->CurrentLeftShoulderWeapon->bReloading;
			}
		}

		if(PlayerChar->CurrentRightShoulderWeapon != NULL)
		{
			if (bRightShoulderWeaponReloading)
			{
				PlayReload(EEquipSlot::RIGHT_SHOULDER, PlayerChar->CurrentRightShoulderWeapon);
				bRightShoulderWeaponReloading = PlayerChar->CurrentRightShoulderWeapon->bReloading;
				if (!bRightShoulderWeaponReloading)
				{
					SetWeaponBarSlot(EEquipSlot::RIGHT_SHOULDER, PlayerChar->CurrentRightShoulderWeapon);
				}
			}

			if(!bRightShoulderWeaponReloading)
			{

				RightShoulderWeaponAmmoCount->SetText(FText::FromString(
					FString::FromInt(PlayerChar->CurrentRightShoulderWeapon->CurrentAmmoCount) + "/" +
					FString::FromInt(PlayerChar->CurrentRightShoulderWeapon->MagazineSize)));
				RightShoulderWeaponTotalAmmoCount->SetText(FText::FromString(FString::FromInt(PlayerChar->CurrentRightShoulderWeapon->TotalAmmoCount)));
				bRightShoulderWeaponReloading = PlayerChar->CurrentRightShoulderWeapon->bReloading;
				
			}
		}

	}
}

void UGOLMInGameHudWidget::SetWeaponBarSlot(EEquipSlot WeaponSlot, AWeapon *NewWeapon)
{
	switch (WeaponSlot)
	{
	case EEquipSlot::HAND_SLOT:
		if (NewWeapon == NULL)
		{
			HandWeaponImage->SetBrushFromTexture(NullImage);
			HandWeaponName->SetText(FText::FromString(""));
			HandWeaponAmmoCount->SetText(FText::FromString(""));
			HandWeaponTotalAmmoCount->SetText(FText::FromString(""));
		}
		else
		{
			HandWeaponImage->SetBrushFromTexture(NewWeapon->WeaponImage);

			HandWeaponName->SetText(FText::FromString(NewWeapon->Name.ToString()));
			HandWeaponAmmoCount->SetText(FText::FromString(
				FString::FromInt(NewWeapon->CurrentAmmoCount) + "/" +
				FString::FromInt(NewWeapon->MagazineSize)));

			HandWeaponTotalAmmoCount->SetText(FText::FromString(FString::FromInt(NewWeapon->TotalAmmoCount)));
		}
		break;
	case EEquipSlot::LEFT_SHOULDER:
		if (NewWeapon == NULL)
		{
			LeftShoulderWeaponImage->SetBrushFromTexture(NullImage);
			LeftShoulderWeaponWeaponName->SetText(FText::FromString(""));
			LeftShoulderWeaponWeaponAmmoCount->SetText(FText::FromString(""));
			LeftShoulderWeaponWeaponTotalAmmoCount->SetText(FText::FromString(""));
		}
		else
		{
			LeftShoulderWeaponImage->SetBrushFromTexture(NewWeapon->WeaponImage);
			LeftShoulderWeaponWeaponName->SetText(FText::FromString(NewWeapon->Name.ToString()));
			LeftShoulderWeaponWeaponAmmoCount->SetText(FText::FromString(
				FString::FromInt(NewWeapon->CurrentAmmoCount) + "/" +
				FString::FromInt(NewWeapon->MagazineSize)));
			LeftShoulderWeaponWeaponTotalAmmoCount->SetText(FText::FromString(FString::FromInt(NewWeapon->TotalAmmoCount)));
		}
		break;
	case EEquipSlot::RIGHT_SHOULDER:
		if (NewWeapon == NULL)
		{
			RightShoulderWeaponImage->SetBrushFromTexture(NullImage);
			RightShoulderWeaponName->SetText(FText::FromString(""));
			RightShoulderWeaponAmmoCount->SetText(FText::FromString(""));
			RightShoulderWeaponTotalAmmoCount->SetText(FText::FromString(""));
		}
		else
		{
			RightShoulderWeaponImage->SetBrushFromTexture(NewWeapon->WeaponImage);
			RightShoulderWeaponName->SetText(FText::FromString(NewWeapon->Name.ToString()));
			RightShoulderWeaponAmmoCount->SetText(FText::FromString(
				FString::FromInt(NewWeapon->CurrentAmmoCount) + "/" +
				FString::FromInt(NewWeapon->MagazineSize)));
			RightShoulderWeaponTotalAmmoCount->SetText(FText::FromString(FString::FromInt(NewWeapon->TotalAmmoCount)));
		}
		break;
	default:
		break;

	}
}

void UGOLMInGameHudWidget::SetWeaponSlotUI(
	EEquipSlot WeaponSlot, 
	UImage *WeaponImage, 
	UTextBlock *AmmoCount, 
	UTextBlock *TotalAmmoCount, 
	UTextBlock *WeaponName,
	UImage *WeaponHighlight)
{
	switch (WeaponSlot)
	{
	case EEquipSlot::HAND_SLOT:
		HandWeaponImage = WeaponImage;
		HandWeaponName = WeaponName;
		HandWeaponAmmoCount = AmmoCount;
		HandWeaponTotalAmmoCount = TotalAmmoCount;
		HandWeaponHighlight = WeaponHighlight;
		break;
	case EEquipSlot::LEFT_SHOULDER:
		LeftShoulderWeaponImage = WeaponImage;
		LeftShoulderWeaponWeaponName = WeaponName;
		LeftShoulderWeaponWeaponAmmoCount = AmmoCount;
		LeftShoulderWeaponWeaponTotalAmmoCount = TotalAmmoCount;
		LeftShoulderWeaponHighlight = WeaponHighlight;
		break;
	case EEquipSlot::RIGHT_SHOULDER:
		RightShoulderWeaponImage = WeaponImage;
		RightShoulderWeaponName = WeaponName;
		RightShoulderWeaponAmmoCount = AmmoCount;
		RightShoulderWeaponTotalAmmoCount = TotalAmmoCount;
		RightShoulderWeaponHighlight = WeaponHighlight;
		break;
	default:
		break;
	}
}
void UGOLMInGameHudWidget::SelectCurrentWeapon(EEquipSlot WeaponSlot)
{
	CurrentSelection = WeaponSlot;
	FLinearColor Color = FLinearColor(0, 1, 1, 1);
	FLinearColor Transparent = FLinearColor(0, 1, 1, 0);
	switch (WeaponSlot)
	{
	case EEquipSlot::HAND_SLOT:
		HandWeaponHighlight->SetColorAndOpacity(Color);
		LeftShoulderWeaponHighlight->SetColorAndOpacity(Transparent);
		RightShoulderWeaponHighlight->SetColorAndOpacity(Transparent);
		break;
	case EEquipSlot::LEFT_SHOULDER:
		HandWeaponHighlight->SetColorAndOpacity(Transparent);
		LeftShoulderWeaponHighlight->SetColorAndOpacity(Color);
		RightShoulderWeaponHighlight->SetColorAndOpacity(Transparent);
		break;
	case EEquipSlot::RIGHT_SHOULDER:
		HandWeaponHighlight->SetColorAndOpacity(Transparent);
		LeftShoulderWeaponHighlight->SetColorAndOpacity(Transparent);
		RightShoulderWeaponHighlight->SetColorAndOpacity(Color);
		break;
	default:
		break;
	}

}
EEquipSlot UGOLMInGameHudWidget::GetCurrentWeaponSelection()
{
	return CurrentSelection;
}

void UGOLMInGameHudWidget::PlayReload(EEquipSlot WeaponSlot, AWeapon *CurrentWeapon)
{
	switch (WeaponSlot)
	{
	case EEquipSlot::HAND_SLOT:
		HandWeaponImage->SetBrushFromTexture(ReloadImage);
		HandWeaponAmmoCount->SetText(FText::FromString(FString::SanitizeFloat(CurrentWeapon->ReloadTimeFinish - CurrentWeapon->ReloadTimer).Left(3)));
		break;
	case EEquipSlot::LEFT_SHOULDER:
		LeftShoulderWeaponImage->SetBrushFromTexture(ReloadImage);
		LeftShoulderWeaponWeaponAmmoCount->SetText(FText::FromString(FString::SanitizeFloat(CurrentWeapon->ReloadTimeFinish - CurrentWeapon->ReloadTimer).Left(3)));

		break;
	case EEquipSlot::RIGHT_SHOULDER:
		RightShoulderWeaponImage->SetBrushFromTexture(ReloadImage);
		RightShoulderWeaponAmmoCount->SetText(FText::FromString(FString::SanitizeFloat(CurrentWeapon->ReloadTimeFinish - CurrentWeapon->ReloadTimer).Left(3)));
		break;
	default:
		break;
	}
}
void UGOLMInGameHudWidget::SetReturnHomeCooldownUI(UImage *Image, UTextBlock *CooldownTimer)
{
	ReturnHomeImage = Image;
	ReturnHomeTimer = CooldownTimer;
}
void UGOLMInGameHudWidget::UpdateReturnHomeCooldownUI()
{
	AGOLMCharacter *PlayerChar = Cast<AGOLMCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (PlayerChar != NULL)
	{
		if (PlayerChar->bCanGoHome)
		{
			ReturnHomeImage->SetBrushFromTexture(CanReturnHome);
			ReturnHomeTimer->SetText(FText::FromString(""));
		}
		else
		{
			ReturnHomeImage->SetBrushFromTexture(CantReturnHome);
			ReturnHomeTimer->SetText(FText::FromString(FString::SanitizeFloat(PlayerChar->ReturnHomeCoolDown - PlayerChar->ReturnHomeTimer).Left(3)));
		}
	}
}