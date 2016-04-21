// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMPlayerController.h"
#include "Weapon.h"
#include "GOLMEquipmentMenuItemWeapon.generated.h"


/**
 * 
 */
UCLASS()
class GOLM_API UGOLMEquipmentMenuItemWeapon : public UUserWidget
{
	GENERATED_BODY()
		AWeapon *Weapon;
public:
	void SetWeapon(AWeapon *SavedWeapon);

	UFUNCTION(BlueprintCallable, Category = Action)	void GetWeapon();
	UFUNCTION(BlueprintCallable, Category = Data) FName GetWeaponName();
	UFUNCTION(BlueprintCallable, Category = Data) UTexture2D *GetImage();
};
