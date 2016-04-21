// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Projectile.h"
#include "GOLMEquipmentMenuItemProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMEquipmentMenuItemProjectile : public UUserWidget
{
	GENERATED_BODY()

private:
	AProjectile *CurrentProjectile;
	
public:
	void SetProjectile(AProjectile *NewProjectile);
	UFUNCTION(BlueprintCallable, Category = Action) void GetProjectile();
	UFUNCTION(BlueprintCallable, Category = Data) FName GetProjectileName();
	UFUNCTION(BlueprintCallable, Category = Data) UTexture2D *GetProjectileImage();
	
};
