// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOLMPlayerState.h"
#include "GOLMPlayerController.h"
#include "GOLMCharacter.h"
#include "GameFramework/GameMode.h"
#include "GOLMGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API AGOLMGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AGOLMGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerCharacters) TSubclassOf<AGOLMCharacter> PlayerCharacterRegular;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerCharacters) TArray<TSubclassOf<AGOLMCharacter>> EnemyAISpawning;

	UFUNCTION(BlueprintCallable, Category = ServerActions)
		void RequestRespawn(APlayerController *RequestingPlayer);

	UFUNCTION(BlueprintCallable, Category = ServerActions)
		void GotoSpawnLocation(FName PlayerStartTag, AGOLMCharacter *RequestingPlayerCharacter);

	//For Testing Purposes
	UFUNCTION(BlueprintCallable, Category = ServerStart)
		void StartDedicatedServer();

protected:
	UFUNCTION()
		virtual void PostLogin(APlayerController *NewPlayer) override;

	UFUNCTION()
		virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		virtual void Logout(AController* Exiting) override;

	UFUNCTION()
		virtual void BeginPlay() override;

private:
	bool IsLevel(FString LevelName);
	bool bAllLevelLoaded;
	TArray<class AGOLMPlayerStart*> EnemySpawnLocations;
	TArray<class AGOLMPlayerStart*> PlayerSpawnLocations;
	class AGOLMPlayerStart* LockerRoomLocation;

	
	float EnemySpawnTimer;

	
	class AGOLMGameState *CurrentGameState;


public:
	//Server Infomation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData) int32 MaxNumOfEnemies;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData) float EnemySpawnInterval;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData)	FName NameOfSession;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData)	bool IsLanServer;

	UFUNCTION(BlueprintCallable, Category = SpawnEnemy)
		void SpawnEnemy();

	UFUNCTION(BlueprintCallable, Category = Init)
		void GetSpawnLocations();

	FVector GetPlayerRandomSpawnLocation();
	FVector GetLockerRoomSpawnLocation();

	void Init();
	bool bHasInitialized;

	void KillAllEnemies();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemiesTracking) TArray<AGOLMCharacter*> Enemies;

	bool bIsThereKyle;

	int32 NumOfEnemies;
	int32 NumOfPlayers;

	float MatchLengthinSeconds;
	bool GameIsDone;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameStatus) bool GameStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameTime)
		float MaxGameTime;
	float CurrentGameTime;
	bool IsGameOver;

	float UpdateTimeEvent;

	void FindWinner();



protected:
	
	
	
};
