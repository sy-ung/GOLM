// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GOLMCharacter.h"
#include "GOLMPlayerState.h"
#include "GOLMGameState.h"
#include "GameFramework/PlayerController.h"
#include "GOLMPlayerController.generated.h"


UCLASS()
class AGOLMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGOLMPlayerController();


	UFUNCTION()
		virtual void BeginPlay() override;

	UFUNCTION()
		virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void FireWeapon(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void Aim();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void RotatePlayerCamera(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MovePlayerCamera(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MovePlayerUp(bool value);
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MovePlayerDown(bool value);
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MovePlayerLeft(bool value);
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void MovePlayerRight(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void BoostPlayer(bool value);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBoostPlayer(bool value);
		void ServerBoostPlayer_Implementation(bool value);
		bool ServerBoostPlayer_Validate(bool value);


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void ZoomPlayerCamera(float deltaZoom);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		FVector GetMouseHit();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void ChangeWeapon(EGetWeapon NewWeapon, EEquipSlot Slot);
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void GetEquippedWeapons();
	UFUNCTION(Client, Reliable)
		void ClientGetEquippedWeapons();
		void ClientGetEquippedWeapons_Implementation();


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void GotoLockerRoom();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerGotoLockerRoom();
		void ServerGotoLockerRoom_Implementation();
		bool ServerGotoLockerRoom_Validate();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)
		void RespawnCharacter();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = DebugStuffICanDo)
		void ServerRespawn();
		void ServerRespawn_Implementation();
		bool ServerRespawn_Validate();


	UFUNCTION(BlueprintCallable, Category = DebugStuffICanDo)
		void KillCharacter();



	UFUNCTION(BlueprintCallable, Category = Debug)
		void DoDamage(float damage);

protected:

	AGOLMCharacter *PlayerCharacter;
};


