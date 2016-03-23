// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameMode.h"
#include "GOLMGameInstance.h"
#include "Engine/Blueprint.h"

AGOLMGameMode::AGOLMGameMode()
{

}

void AGOLMGameMode::BeginPlay()
{
	//StartDedicatedServer();
	//GEngine->AddOnScreenStart
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
					PlayerCharacter->Init();
					PlayerCharacter->Respawn();
				}
			}

		}
	}
}
void AGOLMGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TArray<AActor*, FDefaultAllocator> STUFF;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), STUFF);

	if (false)
		if (STUFF.Num() != 0)
		{
			//GEngine->ClearOnScreenDebugMessages();
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "There is " + FString::FromInt(STUFF.Num()) + " spawn location");
			for (int32 i = 0; i < STUFF.Num(); i++)
			{
				GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::White, STUFF[i]->GetName());
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

