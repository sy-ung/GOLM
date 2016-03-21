// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMEquipmentMenuItemWeapon.h"
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

	TArray<UGOLMEquipmentMenuItemWeapon*> WeaponListItems;

	bool bListCreated;

public:
	UFUNCTION(BlueprintCallable, Category = Init)	void SetScrollBox(UScrollBox *WeaponListScollBox);
	UFUNCTION(BlueprintCallable, Category = Init)	void SetupWeaponSelection();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponItemWidget)	TSubclassOf<UUserWidget> WeaponItemWidget;

};
