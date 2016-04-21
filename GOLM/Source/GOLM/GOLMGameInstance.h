// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
//#include "OnlineSubsystem.h"
#include "GOLMServerListWidget.h"
#include "GOLMMultiplayerMenuWidget.h"
#include "GOLMGameInstance.generated.h"

/**
 * 
 */

//UENUM(BlueprintType)
//enum class ECurrentGameState : uint8
//{
//	MAIN_MENU		UMETA(DisplyName = "InMainMenu"),
//	OPTIONS			UMETA(DisplayName = "InOptions"),
//	FIND_GAME		UMETA(DisplayName = "InFindGame"),
//	PLAYING			UMETA(DisplayName = "InGame")
//};

UCLASS()
class GOLM_API UGOLMGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UUserWidget *CurrentWidget;
	void SetActiveWidget(TSubclassOf<UUserWidget> NewWidget);

	//***Delegates for when Creating and Starting a session is complete
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	//***Handles to register those Delegates
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	//***Delegate for searching for sessions
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	//***Handle for register searching delegate
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	//***Delegate for joining a session
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	//***Delegate handle for joining
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	//***Delegate for destroying session
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	//***Delegate handle for destroying session
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	
	//***Searching for sessions data
	TSharedPtr<FOnlineSessionSearch> SessionSearchResults;

	//***Settings for our session
	TSharedPtr<FOnlineSessionSettings> SessionSettings;

	//***String values of session found to output to screen
	TArray<FString> SessionsString;

	TArray<UGOLMServerListWidget> ServerListWidgets;

	FSessionToJoin SessionSelected;

	//***UserID - User that started this request
	//***SessionName - Name of the session
	//***bAsLan - is this a lan game?
	//***bAsPresence - is this a presense session?
	//***MaxPlayers - MaxPlayers
	bool HostSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, bool bAsLan, bool bAsPresence, uint32 MaxPlayers);
	
	//***FindSession for ONLINE session
	void FindSessions(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, bool bAsLan, bool bAsPresence);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, const FOnlineSessionSearchResult &SearchResults);

	void RemoveCurrentMenu();
	void ToggleGameInput();

	void CreateServerRowWidget(uint32 index);

protected:

	//***Will get called when a 'session create' request is completed
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//***Will get called when a 'session start' request is completed
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	//***Will get called when a 'session find' request is completed
	void OnFindSessionsComplete(bool bWasSuccessful);

	//***Will get called when a 'session join' request is completed
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//***Will get called when a 'destroy session' requst is completed
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
	UGOLMGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Menus) TSubclassOf<UUserWidget> MainMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Menus) TSubclassOf<UUserWidget> OptionsMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Menus) TSubclassOf<UUserWidget> MultiplayerMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Menus) TSubclassOf<UUserWidget> EquipmentMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Menus) TSubclassOf<UUserWidget> InGameMenuWidget;

	UPROPERTY(BlueprintReadOnly, Category = PlayerData) FName CurrentPlayerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SessionData) float GameTimeInSeconds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SessionData) int32 MaxAIBot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ServerSelected) bool bHasSelectedServer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ServerSelected) bool bCanSearchAgain;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ServerSelected) bool bCanHostGame;


	UFUNCTION(BlueprintCallable, Category = MenuInteractions)	void ShowMainMenu();
	UFUNCTION(BlueprintCallable, Category = MenuInteractions)	void ShowMultiplayerMenu();
	UFUNCTION(BlueprintCallable, Category = MenuInteractions)	void ShowOptionsMenu();
	UFUNCTION(BlueprintCallable, Category = MenuInteractions)	void ShowInGameMenu(bool bToggle);
	UFUNCTION(BlueprintCallable, Category = MenUInteractions)	void ShowEquipmentMenu();

	UFUNCTION(BlueprintCallable, Category = NetworkActions)		void HostGame(bool bAsLan, int32 MaxPlayers, FName GameServerName);
	UFUNCTION(BlueprintCallable, Category = NetworkActions)		void LookForGames();

	UFUNCTION(BlueprintCallable, Category = InstanceActions)	void SetSelectedSession(FSessionToJoin ServerSession);
	UFUNCTION(BlueprintCallable, Category = InstanceActions)	void JoinSelectedSession();
	UFUNCTION(BlueprintCallable, Category = NetworkActions)		void JoinAGame(FSessionToJoin ServerSession);


	UFUNCTION(BlueprintCallable, Category = NetworkActions)		void LeaveGame();

	


};
