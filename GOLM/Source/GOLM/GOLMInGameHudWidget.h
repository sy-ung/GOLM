// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "Blueprint/UserWidget.h"
#include "Weapon.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"



#include "GOLMInGameHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMInGameHudWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UImage *HandWeaponImage;
	UTextBlock *HandWeaponName;
	UTextBlock *HandWeaponAmmoCount;
	UTextBlock *HandWeaponTotalAmmoCount;
	UImage *HandWeaponHighlight;

	UImage *LeftShoulderWeaponImage;
	UTextBlock *LeftShoulderWeaponWeaponName;
	UTextBlock *LeftShoulderWeaponWeaponAmmoCount;
	UTextBlock *LeftShoulderWeaponWeaponTotalAmmoCount;
	UImage *LeftShoulderWeaponHighlight;

	UImage *RightShoulderWeaponImage;
	UTextBlock *RightShoulderWeaponName;
	UTextBlock *RightShoulderWeaponAmmoCount;
	UTextBlock *RightShoulderWeaponTotalAmmoCount;
	UImage *RightShoulderWeaponHighlight;

	UImage *ReturnHomeImage;
	UTextBlock *ReturnHomeTimer;

	EEquipSlot CurrentSelection;


	bool bHandWeaponReloading;
	bool bLeftShoulderWeaponReloading;
	bool bRightShoulderWeaponReloading;

public:

	UFUNCTION(BlueprintCallable, Category = Init) void SetWeaponSlotUI(EEquipSlot WeaponSlot, UImage *WeaponImage, UTextBlock *AmmoCount, UTextBlock *TotalAmmoCount, UTextBlock *WeaponName, UImage *WeaponHighlight);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UIData) UTexture2D *NullImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UIData) UTexture2D *ReloadImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UIData) UTexture2D *CanReturnHome;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UIData) UTexture2D *CantReturnHome;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MapData) UImage *MapImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MapData) UCanvasPanelSlot *MapCanvas;

	UFUNCTION(BlueprintCallable, Category = MapFunction) void UpdateMap();
	UFUNCTION(BlueprintCallable, Category = MapFunction) void MapZoom(float value);

	UFUNCTION(BlueprintCallable, Category = WeaponBarFunction) void UpdateWeaponBar();
	UFUNCTION(BlueprintCallable, Category = WeaponBarFunction) void SetWeaponBarSlot(EEquipSlot WeaponSlot, AWeapon *NewWeapon);
	
	UFUNCTION(BlueprintCallable, Category = WeaponBarFunction) void SelectCurrentWeapon(EEquipSlot WeaponSlot);
	UFUNCTION(BlueprintCallable, Category = WeaponBarFunction) EEquipSlot GetCurrentWeaponSelection();

	UFUNCTION(BlueprintCallable, Category = WeaponBarFunction) void PlayReload(EEquipSlot WeaponSlot, AWeapon *CurrentWeapon);

	UFUNCTION(BlueprintCallable, Category = CoolDowns) void UpdateReturnHomeCooldownUI();
	UFUNCTION(BlueprintCallable, Category = Init) void SetReturnHomeCooldownUI(UImage *Image, UTextBlock *CooldownTimer);
};
