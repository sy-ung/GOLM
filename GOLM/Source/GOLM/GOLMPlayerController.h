// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GOLMCharacter.h"
#include "GOLMPlayerState.h"
#include "GOLMGameState.h"

#include "GameFramework/PlayerController.h"
#include "GOLMPlayerController.generated.h"

UENUM(BlueprintType)
enum class  EPlayerCursorType
{
	CROSSHAIR	UMETA(DisplayName = "Crosshair"),
	MENU		UMETA(DisplayName = "Menu Cursor")
};

UCLASS()
class AGOLMPlayerController : public APlayerController
{
	GENERATED_BODY()



public:
	AGOLMPlayerController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cursor)	class TSubclassOf<UUserWidget> CursorWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cursor)	class TSubclassOf<UUserWidget> EquipmentMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cursor)	class TSubclassOf<UUserWidget> InGameSettingsMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cursor)	class TSubclassOf<UUserWidget> InGameHUD;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cursor)	UTexture2D *Crosshair;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cursor)	UTexture2D *MenuCursor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Menu)	bool bIsInEquipmentMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Menu)	bool bIsInSettingsMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterMode)	
																	bool bIsArcFireOn;
	

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

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void ChangeWeapon(AWeapon *NewWeapon, EEquipSlot Slot);
	
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

	UFUNCTION(BlueprintCallable, Category = Cursor)			void ChangeCursor(EPlayerCursorType NewCursor);
	
	UFUNCTION(BlueprintCallable, Category = EquipmentMenuInteraction)
															void MenuSetWeaponSlotChoice(EEquipSlot Choice);

	UFUNCTION(BlueprintCallable, Category = EquipmentMenuInteraction)
															void ShowEquipmentMenu();
	UFUNCTION(BlueprintCallable, Category = EquipmentMenuInteraction)
															void HideEquipmentMenu();

	UFUNCTION(BlueprintCallable, Category = EquipmentMenuInteraction)
															void ShowInGameSettingsMenu();

	UFUNCTION(BlueprintCallable, Category = EquipmentMenuInteraction)
															void HideInGameSettingsMenu();

	UFUNCTION(BlueprintCallable, Category = InGameHudFunction)
															void HideInGameHud();
	UFUNCTION(BlueprintCallable, Category = InGameHudFunction)
															void ShowInGameHud();

	UFUNCTION(BlueprintCallable, Category = InGameHudFunction)
															void ZoomMiniMap(float value);

	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoPlayerCamera();
	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoFrontCamera();
	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoLeftShoulderCamera();
	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoRightShoulderCamera();
	UFUNCTION(BlueprintCallable, Category = FireMode)		void SetArcFire(bool value);


															


//Non Unreal Engine 4 Application
public:
	EPlayerCursorType CurrentCursorType;
	EEquipSlot GetMenuWeaponSlotChoice();

	class UUserWidget *CursorWidgetReference;
	class UUserWidget *EquipmentMenuReference;
	class UUserWidget *InGameSettingsMenuReference;
	class UUserWidget *InGameHUDReference;

private:
	EEquipSlot MenuSlotChoice;
protected:
	AGOLMCharacter *PlayerCharacter;
};


