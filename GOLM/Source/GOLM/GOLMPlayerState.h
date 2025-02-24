// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"

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
	HAND_SLOT		UMETA(DisplayName = "Hand"),
	RIGHT_SHOULDER	UMETA(DisplayName = "Right Shoulder"),
	LEFT_SHOULDER	UMETA(DisplayName = "Left Shoulder"),
	
	//Use only on weapon config
	ALL				UMETA(DisplayName = "All slots"),
	//Use only on weapon config
	HAND_LEFT		UMETA(DisplayName = "Hand and Left Shoulder weapon slots"),
	//Use only on weapon config
	HAND_RIGHT		UMETA(DisplayName = "Hand and Right Shoulder weapon slots"),
	//Use only on weapon config
	LEFT_RIGHT		UMETA(DisplayName = "Left and Right Shoulder weapon slots")
};

UCLASS()
class GOLM_API AGOLMPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

public:

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = PlayerStateAction)
		class AWeapon *GetWeaponFor(EEquipSlot slot);

	UFUNCTION(BlueprintCallable, Category = PlayerStateAction)
		void SetWeaponFor(class AWeapon *Weapon, EEquipSlot Slot);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetWeaponFor(class AWeapon *Weapon, EEquipSlot Slot);
		void ServerSetWeaponFor_Implementation(class AWeapon *Weapon, EEquipSlot Slot);
		bool ServerSetWeaponFor_Validate(class AWeapon *Weapon, EEquipSlot Slot);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ListOfWeapons)
		TArray<TSubclassOf<class AWeapon>> WeaponsList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ListOfWeapons)
		TArray<USkeletalMesh*> SkinList;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = PlayerData)
		int32 PlayerKills;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = PlayerData)
		int32 PlayerDeath;

	UPROPERTY(Replicated)
		class AWeapon *HandWeapon;

	UPROPERTY(Replicated)
		class AWeapon *RightShoulder;

	UPROPERTY(Replicated)
		class AWeapon *LeftShoulder;

	UPROPERTY(VisibleAnywhere,Replicated,BlueprintReadOnly)
		bool bIsKyle;


	UPROPERTY(Replicated)
		bool isWinner;
	UPROPERTY(Replicated)
		bool isTied;
	UPROPERTY(Replicated)
		bool isLoser;

	void AddToScore(int32 AdditionalKill, int32 AdditionalDeath);

private:




};
