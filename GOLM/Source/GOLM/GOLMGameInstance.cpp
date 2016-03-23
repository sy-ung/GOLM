// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameInstance.h"
#include "GOLMGameMode.h"
#include "Engine.h"

UGOLMGameInstance::UGOLMGameInstance()
{
	//***Binding delegate for Creating Session
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UGOLMGameInstance::OnCreateSessionComplete);

	//***Bind delegate for Starting Session
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UGOLMGameInstance::OnStartOnlineGameComplete);

	//***Bind delegate for Search Session
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UGOLMGameInstance::OnFindSessionsComplete);

	//***Bind Delegate for join session
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UGOLMGameInstance::OnJoinSessionComplete);

	//***Bind Delegate for Destroy Session
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UGOLMGameInstance::OnDestroySessionComplete);
}





void UGOLMGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red,
		FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful)
		);*/
	//***Get Online subsystem for Session interface
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();
	if (OnlineSubSys)
	{
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//***Clear SessionComplete delegate handle since it is finished
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				//***Set StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UGOLMGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, 
		FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful)
		);*/

	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();
	if (OnlineSubSys)
	{
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//***Clear StartSession delegate handle since it is finished
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}
	if (bWasSuccessful)
	{
		ToggleGameInput();

		//***What happens when a player successfully joins a server.

		FLatentActionInfo LatInfo;
		//UGameplayStatics::LoadStreamLevel(GetWorld(), "MainGame", true, false, LatInfo);
		UGameplayStatics::OpenLevel(GetWorld(), "MainGame", true, "listen");
		

		//***NEED TO FIND A WAY TO LISTEN FOR CONNECTION

		//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("GAME SESSION HAS STARTED"));

	}

}



void UGOLMGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, 
		FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful)
		);*/

	//***Get current OnlineSubsystem
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();
	if (OnlineSubSys)
	{
		//***Get Session Interface
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		
		if (Sessions.IsUnique())
		{
			//***Clear OnDestroyCompleteDelegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				//***What do you want to do when you close a Session
				UGameplayStatics::OpenLevel(GetWorld(), "Beginning", true);
				SetActiveWidget(MultiplayerMenuWidget);
			}
		}
	}
}

void UGOLMGameInstance::ShowMainMenu()
{
	if (MainMenuWidget != NULL)
		SetActiveWidget(MainMenuWidget);
	else
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Main Menu Widget is null");
}

void UGOLMGameInstance::ShowMultiplayerMenu()
{
	if(MultiplayerMenuWidget != NULL)
		SetActiveWidget(MultiplayerMenuWidget);
	else
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Multiplayer Menu Widget is null");
}
void UGOLMGameInstance::ShowOptionsMenu()
{
	if (OptionsMenuWidget != NULL)
		SetActiveWidget(OptionsMenuWidget);
	else
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Options Menu Widget is null");
}
void UGOLMGameInstance::ShowInGameMenu(bool bToggle)
{
	if (bToggle)
		SetActiveWidget(InGameMenuWidget);

	if (!bToggle)
		RemoveCurrentMenu();

}
void UGOLMGameInstance::ShowEquipmentMenu()
{

}

void UGOLMGameInstance::RemoveCurrentMenu()
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromViewport();
	}
}

void UGOLMGameInstance::SetActiveWidget(TSubclassOf<UUserWidget> NewWidget)
{
	RemoveCurrentMenu();


	if(NewWidget == NULL)
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "New Widget is null");

	APlayerController *PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerCon == NULL)
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "PlayerController is null");

	CurrentWidget = CreateWidget<UUserWidget>(PlayerCon, NewWidget.GetDefaultObject()->GetClass());

	
	if(CurrentWidget != NULL)
	{
		FInputModeUIOnly UI;
		UI.SetWidgetToFocus(CurrentWidget->GetCachedWidget());
		PlayerCon->SetInputMode(UI);
		CurrentWidget->AddToViewport(0);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Current Widget is NULL");
	}

}

void UGOLMGameInstance::ToggleGameInput()
{
	APlayerController *PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly GI;
	PlayerCon->SetInputMode(GI);
}

void UGOLMGameInstance::HostAGame(bool bAsLan, int32 MaxPlayers)
{
	//Create local player to get UserID from
	ULocalPlayer *const Player = GetFirstGamePlayer();

	HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, bAsLan, true, MaxPlayers);
}

bool UGOLMGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, bool bAsLan, bool bAsPresence, uint32 MaxPlayers)
{
	//***Get Online subsystem to use
	IOnlineSubsystem *const OnlineSubSys = IOnlineSubsystem::Get();
	
	if (OnlineSubSys)
	{
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();

		if (Sessions.IsValid() && UserID.IsValid())
		{
			//***Set up Session settings
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bAsLan;
			SessionSettings->bUsesPresence = bAsPresence;
			SessionSettings->NumPublicConnections = MaxPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			

			SessionSettings->Set(SETTING_MAPNAME, FString("MyGame"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			//***Set delegate handle to session interface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("SessionCreated"));
			return Sessions->CreateSession(*UserID, SessionName, *SessionSettings);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}
	return false;
}

void UGOLMGameInstance::FindSessions(TSharedPtr<const FUniqueNetId>UserID, FName SessionName, bool bAsLan, bool bAsPresence)
{
	//***Get current online subsystem
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();

	if (OnlineSubSys)
	{
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface(); 
		if (Sessions.IsValid() && UserID.IsValid())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "LOOKING FOR A GAME");
			//***SessionSearch settings
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bAsLan;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;
			if (bAsPresence)
			{
				//***Query settings to use for finding matching servers
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bAsPresence, EOnlineComparisonOp::Equals);			
			}

			//***Getting a reference to SessionSearch
			TSharedRef<FOnlineSessionSearch> SearchSettingRef = SessionSearch.ToSharedRef();

			//***Setting delegate to delegate handle of FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			//***Calls the SessionInterface function and OnfindSession delegate gets called after it is completed
			Sessions->FindSessions(*UserID, SearchSettingRef);

		}
	}
	else
	{
		//If something goes wrong, send a false.
		OnFindSessionsComplete(false);
	}
}

void UGOLMGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
	//	FString::Printf(TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful));
	bCanSearchAgain = true;
	//***Get Online SubSystem to work with
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();
	if (OnlineSubSys)
	{
		//***Get Online subsystem interface as a session ptr
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//***Clearing delegate handle of OnFindSession
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			//***If at least found 1 search results
			if (SessionSearch->SearchResults.Num() > 0)
			{
				SessionSearch->SortSearchResults();
				
				//**********FOR LOOP TO ADD SERVER LIST WIDGET***************
				for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
				{

					
					//Current Widget will refer to mulitplayer menu widget
				 	Cast<UGOLMMultiplayerMenuWidget>(CurrentWidget)->AddServer(
						&SessionSearch->SearchResults[i],GetWorld()
						);
				}
				
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "There are no games available");
		}
	}

}


bool UGOLMGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserID, FName SessionName, const FOnlineSessionSearchResult &SearchResults)
{
	//***Get Online SubSystem to work with
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();
	
	if (OnlineSubSys)
	{
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		if (Sessions.IsValid() && UserID.IsValid())
		{
			//***Set the delegate handle for OnJoinSessionComplete
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			return Sessions->JoinSession(*UserID, SessionName, SearchResults);
		}
		else
			return false;
	}
	else
		return false;


}

void UGOLMGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red,
	//	FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result))
	//	);


	//***Getting the Online SubSystem to use
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();

	if (OnlineSubSys)
	{
		//***Getting Sessions
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//***Clear Delegate handle OnJoinSessionCompleteDelegateHandle
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			//***Get Local player controller to "ClientTravel" to the Server Map
			APlayerController *const PlayerController = GetFirstLocalPlayerController();

			//***Client Travel takes a string and can let SessionInterface make an string by using the SessionName and
			//empty string. Every OnlineSubsystem uses different TravelURL
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				ToggleGameInput();
				//***Get the travelURL and use it to ClientTravel to the server
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
				//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::White, "JOINED GAME SUCCESSFULLY");
				//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, "Travel URL is: " + TravelURL);

			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, "JOINED GAME UNSUCCESSFULLY");
			}

		}
	}
	

}

void UGOLMGameInstance::LookForGames()
{
	ULocalPlayer *const Player = GetFirstGamePlayer();
	bCanSearchAgain = false;
	FindSessions(Player->GetPreferredUniqueNetId(), GameSessionName, true, true);


}

void UGOLMGameInstance::SetSelectedSession(FSessionToJoin ServerSession)
{

	SessionSelected = ServerSession;
	//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "From GameInstance " + SessionSelected.GameSession->Session.OwningUserName);
}

void UGOLMGameInstance::JoinSelectedSession()
{
	if (SessionSelected.GameSession != nullptr)
	{
		JoinAGame(SessionSelected);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Session Unavailable");
	}
}

void UGOLMGameInstance::JoinAGame(FSessionToJoin ServerSession)
{
	ULocalPlayer *const Player = GetFirstGamePlayer();

	JoinSession(Player->GetPreferredUniqueNetId(), GameSessionName,*ServerSession.GameSession);
}

void UGOLMGameInstance::LeaveGame()
{
	IOnlineSubsystem *OnlineSubSys = IOnlineSubsystem::Get();
	if (OnlineSubSys)
	{
		IOnlineSessionPtr Sessions = OnlineSubSys->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UGOLMGameInstance::CreateServerRowWidget(uint32 index)
{

}