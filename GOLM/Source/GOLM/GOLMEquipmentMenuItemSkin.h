// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMPlayerController.h"
#include "GOLMEquipmentMenuItemSkin.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMEquipmentMenuItemSkin : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetSkinColor(USkeletalMesh *NewSkin);

	UFUNCTION(BlueprintCallable, Category = Action)	void GetSkin();
	UFUNCTION(BlueprintCallable, Category = Data) FName GetSkinName();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Data) FColor SkinColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Data) USkeletalMesh *CurrentSkin;
};
