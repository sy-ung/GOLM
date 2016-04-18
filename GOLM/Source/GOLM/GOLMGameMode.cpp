// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameMode.h"
#include "GOLMGameInstance.h"
#include "GOLMPlayerStart.h"
#include "Engine/Blueprint.h"

AGOLMGameMode::AGOLMGameMode()
{
	EnemySpawnTimer = 1.0f;
	NumOfEnemies = 0;
	MaxNumOfEnemies = 25;
	EnemySpawnInterval = 0.50f;
	NumOfPlayers = 1;
}

void AGOLMGameMode::BeginPlay()
{
	//StartDedicatedServer();
	//GEngine->AddOnScreenStart
	GetEnemySpawnLocations();
}

void AGOLMGameMode::PostLogin(APlayerController *NewPlayer)
{

	if (IsValid(this))//May have to delete this if game crashes
	{
		Super::PostLogin(NewPlayer);
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
					//PlayerCharacter->SetOwner(RequestingPlayer);
					Cast<AGOLMPlayerController>(RequestingPlayer)->GetCharacterName();
					PlayerCharacter->Init();
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

	if (NumOfEnemies < MaxNumOfEnemies )
	{
		
		EnemySpawnTimer -= DeltaSeconds;
		if (EnemySpawnTimer <= 0)
		{
			SpawnEnemy();
			EnemySpawnTimer = EnemySpawnInterval;
		}
	}
}

void AGOLMGameMode::GotoSpawnLocation(FName PlayerStartTag, AGOLMCharacter *RequestingPlayerCharacter)
{
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::White, PlayerStartTag.ToString());
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
		Enemies.Add(GetWorld()->SpawnActor<AGOLMCharacter>(EnemyAI.GetDefaultObject()->GetClass(), RandomEnemySpawn->GetSpawnLocation(),FRotator(0,0,0)));
		NumOfEnemies++;
	}
}

void AGOLMGameMode::GetEnemySpawnLocations()
{
	TArray<AActor*,FDefaultAllocator> GetSpawnLocs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGOLMPlayerStart::StaticClass(), GetSpawnLocs);
	for (int32 i = 0; i < GetSpawnLocs.Num(); i++)
	{
		AGOLMPlayerStart *StartCheck = Cast<AGOLMPlayerStart>(GetSpawnLocs[i]);
		if(StartCheck->PlayerStartTag == "EnemySpawn")
			EnemySpawnLocations.Add(StartCheck);
	}

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