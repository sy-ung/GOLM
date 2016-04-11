// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMEquipmentMenuItemWeapon.h"
#include "GOLMEquipmentMenuItemProjectile.h"
#include "GOLMEquipmentMenuItemSkin.h"
#include "GOLMPlayerState.h"

#include "Weapon.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GOLMEquipmentMenuWidget.generated.h"

/**
 * 
 */

UCLASS()
class GOLM_API UGOLMEquipmentMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	AGOLMPlayerController *PlayerCon;
	UScrollBox *WeaponScrollBox;
	UScrollBox *ProjectileScrollBox;
	UScrollBox *SkinScrollBox;

	bool bListCreated;

public:
	UFUNCTION(BlueprintCallable, Category = Init)	void SetWeaponScrollBox(UScrollBox *WeaponListScollBox);
	UFUNCTION(BlueprintCallable, Category = Init)	void SetProjectileScrollBox(UScrollBox *ProjectileListScollBox);
	UFUNCTION(BlueprintCallable, Category = Init)	void SetSkinScrollBox(UScrollBox *SkinListScollBox);

	UFUNCTION(BlueprintCallable, Category = InMenu)	void AddToWeaponSelection(TSubclassOf<AWeapon> NewWeapon);

	UFUNCTION(BlueprintCallable, Category = InMenu)	void SetupWeaponSelection();
	UFUNCTION(BlueprintCallable, Category = InMenu)	void SetupWeaponProjectileSelection();
	UFUNCTION(BlueprintCallable, Category = InMenu)	void SetupSkinSelection();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemWidget)	TSubclassOf<UUserWidget> WeaponItemWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemWidget)	TSubclassOf<UUserWidget> ProjectileItemWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemWidget)	TSubclassOf<UUserWidget> SkinItemWidget;
};
