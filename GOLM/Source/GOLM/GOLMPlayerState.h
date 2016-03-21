// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Weapon.h"
#include "GOLMPlayerState.generated.h"

//UENUM(BlueprintType)
//enum class EGetWeapon :uint8
//{
//	NONE				UMETA(DisplayName = "NONE"),
//	ROCKET_LAUNCHER		UMETA(DisplayName = "RocketLauncher"),
//	RIFLE				UMETA(DisplayName = "Rifle")
//};

UENUM(BlueprintType)
enum class EEquipSlot :uint8
{
	HAND_SLOT		UMETA(DisplayName = "Hand Weapon"),
	RIGHT_SHOULDER	UMETA(DisplayName = "Right Shoulder"),
	LEFT_SHOULDER	UMETA(DisplayName = "Left Shoulder")
};

UCLASS()
class GOLM_API AGOLMPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

public:

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
		float Health;
	
	UFUNCTION(BlueprintCallable, Category = PlayerStateInformation)
		float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = PlayerStateAction)
		void ResetPlayer();

	UFUNCTION()
		virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = PlayerStateAction)
		AWeapon *GetWeaponFor(EEquipSlot slot);

	UFUNCTION(BlueprintCallable, Category = PlayerStateAction)
		void SetWeaponFor(AWeapon *Weapon, EEquipSlot Slot);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ListOfWeapons)
		TArray<TSubclassOf<AWeapon>> WeaponsList;

private:

	AWeapon *HandWeapon;
	AWeapon *RightShoulder;
	AWeapon *LeftShoulder;


};
