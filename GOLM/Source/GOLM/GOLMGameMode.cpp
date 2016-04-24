// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameMode.h"
#include "GOLMGameInstance.h"
#include "GOLMPlayerStart.h"
#include "Engine/Blueprint.h"

AGOLMGameMode::AGOLMGameMode()
{
	EnemySpawnTimer = 10.0f;
	NumOfEnemies = 0;
	MaxNumOfEnemies = 0;
	EnemySpawnInterval = 2.0f;
	NumOfPlayers = 0;
	UpdateTimeEvent = 0;
	bHasInitialized = false;
	MaxGameTime = 10;
	GameStarted = false;
}

void AGOLMGameMode::BeginPlay()
{
	//StartDedicatedServer();
	//GEngine->AddOnScreenStart
	Super::BeginPlay();


}
void AGOLMGameMode::Init()
{
	GetSpawnLocations();
	//MaxGameTime = 3000;
	MaxGameTime = Cast<UGOLMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameTimeInSeconds;
	MaxNumOfEnemies = Cast<UGOLMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->MaxAIBot;
	CurrentGameState = Cast<AGOLMGameState>(GameState);
	CurrentGameTime = MaxGameTime;
	GameStarted = true;
}

void AGOLMGameMode::PostLogin(APlayerController *NewPlayer)
{



	if (IsValid(this))//May have to delete this if game crashes
	{
		Super::PostLogin(NewPlayer);
		if (!bHasInitialized)
		{
			if(IsLevel("GameLevel"))
			{
				Init();
				bHasInitialized = true;
				
			}
		}
		RequestRespawn(NewPlayer);
		
	}
}

void AGOLMGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AGOLMGameMode::RequestRespawn(APlayerController *RequestingPlayer)
{
	if (!IsLevel("Beginning"))
	{
		if (RequestingPlayer)
		{
			AGOLMCharacter *PlayerCharacter = Cast<AGOLMCharacter>(RequestingPlayer->GetCharacter());
			if (PlayerCharacter == NULL)
			{
				PlayerCharacter = GetWorld()->SpawnActor<AGOLMCharacter>(PlayerCharacterRegular);
				if (IsValid(PlayerCharacter))
				{
					RequestingPlayer->UnPossess();
					RequestingPlayer->Possess(PlayerCharacter);
					PlayerCharacter->SetOwner(RequestingPlayer);
					Cast<AGOLMPlayerController>(RequestingPlayer)->GetCharacterName();
					PlayerCharacter->CurrentLevelStream = "LockerRoom";
					//PlayerCharacter->Init();
					PlayerCharacter->Respawn();
					NumOfPlayers++;
				}
			}
		}
	}
}
void AGOLMGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GameStarted)
	{
		if (NumOfEnemies < MaxNumOfEnemies )
		{
		
			EnemySpawnTimer -= DeltaSeconds;
			if (EnemySpawnTimer <= 0)
			{
				SpawnEnemy();
				EnemySpawnTimer = EnemySpawnInterval;
			}
		}

		if(!GameIsDone)
		{
			CurrentGameTime -= DeltaSeconds;
			if (CurrentGameTime <= 0)
			{
				GameIsDone = true;
				CurrentGameState->IsGameOver = true;
			}
			else
			{
				UpdateTimeEvent += DeltaSeconds;
				if(UpdateTimeEvent>=1)
				{
					CurrentGameState->CurrentGameTime = (int)CurrentGameTime;
					UpdateTimeEvent = 0;
				}
			}
		}
		else
		{
			FindWinner();
		}
	}

}

void AGOLMGameMode::GotoSpawnLocation(FName PlayerStartTag, AGOLMCharacter *RequestingPlayerCharacter)
{
	if (!IsLevel("Beginning"))
	{
		TArray<AActor*, FDefaultAllocator> SpawnLocs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnLocs);

		if (SpawnLocs.Num() != 0)
		{
			for (int32 i = 0; i < SpawnLocs.Num(); i++)
			{
				if (Cast<APlayerStart>(SpawnLocs[i])->PlayerStartTag == PlayerStartTag)
				{
					RequestingPlayerCharacter->SetActorLocation(SpawnLocs[i]->GetActorLocation());
					RequestingPlayerCharacter->SetActorRotation(SpawnLocs[i]->GetActorRotation());

					break;
				}
			}
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), "NO SPAWN LOCATIONS");
	}


}

bool AGOLMGameMode::IsLevel(FString LevelName)
{
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == LevelName)
	{
		return true;
	}
	else
		return false;
}

void AGOLMGameMode::StartDedicatedServer()
{
	//IOnlineSubsystem *const OnlineSubSys = IOnlineSubsystem::Get();
	//HostSession( NameOfSession, IsLanServer, true, MaximumPlayers);
	//Cast<UGOLMGameInstance>(GetGameInstance())->HostAGame(true, 8);
}

void AGOLMGameMode::SpawnEnemy()
{
	if (EnemySpawnLocations.Num() != 0)
	{
		AGOLMPlayerStart *RandomEnemySpawn = EnemySpawnLocations[FMath::RandRange(0, EnemySpawnLocations.Num()-1)];
		
		Enemies.Add(
			GetWorld()->SpawnActor<AGOLMCharacter>(
				EnemyAISpawning[FMath::RandRange(0,EnemyAISpawning.Num()-1)].GetDefaultObject()->GetClass(),
				RandomEnemySpawn->GetSpawnLocation(),FRotator(0,0,0)));

		NumOfEnemies++;
	}
}

void AGOLMGameMode::GetSpawnLocations()
{
	TArray<AActor*,FDefaultAllocator> GetSpawnLocs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGOLMPlayerStart::StaticClass(), GetSpawnLocs);
	for (int32 i = 0; i < GetSpawnLocs.Num(); i++)
	{
		AGOLMPlayerStart *StartCheck = Cast<AGOLMPlayerStart>(GetSpawnLocs[i]);
		if(StartCheck->PlayerStartTag == "EnemySpawn")
			EnemySpawnLocations.Add(StartCheck);
		else if (StartCheck->PlayerStartTag == "GameLevel_Random")
			PlayerSpawnLocations.Add(StartCheck);
		else if (StartCheck->PlayerStartTag == "LockerRoom")
			LockerRoomLocation = StartCheck;
	}

}

FVector AGOLMGameMode::GetPlayerRandomSpawnLocation()
{
	return PlayerSpawnLocations[FMath::RandRange(0, PlayerSpawnLocations.Num() - 1)]->GetSpawnLocation();
}
FVector AGOLMGameMode::GetLockerRoomSpawnLocation()
{
	return LockerRoomLocation->GetSpawnLocation();
}

void AGOLMGameMode::KillAllEnemies()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGOLMCharacter::StaticClass(), Enemies);
	for (int32 i = 0; i < Enemies.Num(); i++)
	{
		AGOLMCharacter *RobotCheck = Cast<AGOLMCharacter>(Enemies[i]);
		if (RobotCheck->bIsAI)
		{
			RobotCheck->Destroy();
		}
	}
	Enemies.Empty();
}

void AGOLMGameMode::FindWinner()
{
	TArray<AGOLMPlayerState*> TiedPlayers;
	AGOLMPlayerState *Winner = NULL;
	for (int32 i = 0; i < CurrentGameState->PlayerArray.Num(); i++)
	{
		AGOLMPlayerState *PlayerCheck = Cast<AGOLMPlayerState>(CurrentGameState->PlayerArray[i]);
		if(Winner == NULL)
		{
			Winner = PlayerCheck;
		}
		else
		{
			if (PlayerCheck->PlayerKills == Winner->PlayerKills)
			{
				if (PlayerCheck->PlayerDeath == Winner->PlayerDeath)
				{
					TiedPlayers.Add(PlayerCheck);
					TiedPlayers.Add(Winner);
				}
				else if (PlayerCheck->PlayerDeath < Winner->PlayerDeath)
				{
					Winner->isLoser = true;
					Winner = PlayerCheck;
				}
				else if (PlayerCheck->PlayerDeath > Winner->PlayerDeath)
				{
					PlayerCheck->isLoser = true;
				}
			}
			else if (PlayerCheck->PlayerKills < Winner->PlayerKills)
			{
				PlayerCheck->isLoser = true;
			}
			else
			{
				Winner->isLoser = true;
				Winner = PlayerCheck;
			}
		}
	}
	if(TiedPlayers.Num() > 0)
	{
		for (int32 i = 0; i < TiedPlayers.Num(); i++)
		{
			if (TiedPlayers[i] != Winner)
			{
				if (Winner->PlayerKills == TiedPlayers[i]->PlayerKills)
				{
					if (Winner->PlayerDeath == TiedPlayers[i]->PlayerDeath)
					{
						for (int32 j = 0; j < TiedPlayers.Num(); j++)
						{
							if (TiedPlayers[j] != Winner)
							{
								TiedPlayers.Add(Winner);
							}
							else
								break;	
						}
					}
					else if (Winner->PlayerDeath < TiedPlayers[i]->PlayerDeath)
					{
						TiedPlayers[i]->isLoser = true;
					}
					else if(TiedPlayers[i]->PlayerDeath < Winner->PlayerDeath)
					{
						Winner->isLoser = true;
						Winner = TiedPlayers[i];
					}
				}
				else if(Winner->PlayerKills> TiedPlayers[i]->PlayerKills)
				{
					TiedPlayers[i]->isLoser = true;
				}
				else if (Winner->PlayerKills < TiedPlayers[i]->PlayerKills)
				{
					Winner->isLoser = true;
					Winner = TiedPlayers[i];
				}
			}
		}
	}
	else
	{
		Winner->isWinner = true;
	}
	

}