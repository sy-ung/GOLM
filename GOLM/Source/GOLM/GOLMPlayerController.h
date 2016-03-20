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


	UFUNCTION()												virtual void BeginPlay() override;

	UFUNCTION()												virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void FireWeapon(bool value);
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void RotatePlayerCamera(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerCamera(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerUp(bool value);
	UFUNCTION(Server, Reliable, WithValidation)				void ServerMovePlayerUp(bool value);
															void ServerMovePlayerUp_Implementation(bool value);
															bool ServerMovePlayerUp_Validate(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerDown(bool value);
	UFUNCTION(Server, Reliable, WithValidation)				void ServerMovePlayerDown(bool value);
															void ServerMovePlayerDown_Implementation(bool value);
															bool ServerMovePlayerDown_Validate(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerLeft(bool value);
	UFUNCTION(Server, Reliable, WithValidation)				void ServerMovePlayerLeft(bool value);
															void ServerMovePlayerLeft_Implementation(bool value);
															bool ServerMovePlayerLeft_Validate(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerRight(bool value);
	UFUNCTION(Server, Reliable, WithValidation)				void ServerMovePlayerRight(bool value);
															void ServerMovePlayerRight_Implementation(bool value);
															bool ServerMovePlayerRight_Validate(bool value);


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void BoostPlayer(bool value);
	UFUNCTION(Server, Reliable, WithValidation)				void ServerBoostPlayer(bool value);
															void ServerBoostPlayer_Implementation(bool value);
															bool ServerBoostPlayer_Validate(bool value);




	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void ZoomPlayerCamera(float deltaZoom);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	FVector GetMouseHit();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void ChangeWeapon(EGetWeapon NewWeapon, EEquipSlot Slot);
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void GetEquippedWeapons();

	UFUNCTION(Client, Reliable)								void ClientGetEquippedWeapons();
															void ClientGetEquippedWeapons_Implementation();


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void GotoLockerRoom();
	UFUNCTION(Server, Reliable, WithValidation)				void ServerGotoLockerRoom();
															void ServerGotoLockerRoom_Implementation();
															bool ServerGotoLockerRoom_Validate();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void RespawnCharacter();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = DebugStuffICanDo)
															void ServerRespawn();
															void ServerRespawn_Implementation();
															bool ServerRespawn_Validate();


	UFUNCTION(BlueprintCallable, Category = DebugStuffICanDo)
															void KillCharacter();



	UFUNCTION(BlueprintCallable, Category = Debug)			void DoDamage(float damage);

protected:

	AGOLMCharacter *PlayerCharacter;
};


