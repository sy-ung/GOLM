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




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Menu)	bool bIsInEquipmentMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Menu)	bool bIsInSettingsMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterMode)	
																	bool bIsArcFireOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	bool bMovePlayerCamera;
	

	UFUNCTION()												virtual void BeginPlay() override;

	UFUNCTION()												virtual void Tick(float DeltaSeconds) override;


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void FireWeapon(EEquipSlot WeaponSlot, bool StartShooting);
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void RotatePlayerCamera(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerUp(bool value);

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerDown(bool value);
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerLeft(bool value);
	
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void MovePlayerRight(bool value);



	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void BoostPlayer(bool value);
	UFUNCTION(Server, Reliable, WithValidation)				void ServerBoostPlayer(bool value);
															void ServerBoostPlayer_Implementation(bool value);
															bool ServerBoostPlayer_Validate(bool value);




	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void ZoomPlayerCamera(float deltaZoom);


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	FVector GetMouseHit();
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	FVector GetMouseMovement();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void ChangeWeapon(AWeapon *NewWeapon, EEquipSlot Slot);
	UFUNCTION(BlueprintCallable, Category = StiffICanDo)	void ChangeSkin(USkeletalMesh *NewSkin);
	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void GetEquippedWeapons();

	UFUNCTION(Client, Reliable)								void ClientGetEquippedWeapons();
															void ClientGetEquippedWeapons_Implementation();


	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void GotoLockerRoom();

	UFUNCTION(BlueprintCallable, Category = StuffICanDo)	void GotoLevel(FName LevelName);

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

	UFUNCTION(BlueprintCallable, Category = Init)
		void SetCharacterName(FName NewName);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetCharacterName(FName NewName);
		void ServerSetCharacterName_Implementation(FName NewName);
		bool ServerSetCharacterName_Validate(FName NewName);

	UFUNCTION(BlueprintCallable, Category = Init)
		void GetCharacterName();
	UFUNCTION(Client, Reliable)
		void ClientGetCharacterName();
		void ClientGetCharacterName_Implementation();


	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoPlayerCamera();
	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoFrontCamera();
	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoLeftShoulderCamera();
	UFUNCTION(BlueprintCallable, Category = SetCamera)		void GotoRightShoulderCamera();
	UFUNCTION(BlueprintCallable, Category = FireMode)		void SetArcFire(bool value);


	UPROPERTY(BlueprintReadWrite, Category = PlayerData) FName CurrentPlayerName;


//Non Unreal Engine 4 Application
public:
	EPlayerCursorType CurrentCursorType;
	EEquipSlot GetMenuWeaponSlotChoice();

	class UGOLMMouseWidget *CursorWidgetReference;
	class UGOLMEquipmentMenuWidget *EquipmentMenuReference;
	class UUserWidget *InGameSettingsMenuReference;
	class UGOLMInGameHudWidget *InGameHUDReference;

private:
	EEquipSlot MenuSlotChoice;
protected:
	AGOLMCharacter *PlayerCharacter;
};


