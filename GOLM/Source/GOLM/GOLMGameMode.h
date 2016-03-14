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


public:
	//Server Infomation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData)	FName NameOfSession;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData)	bool IsLanServer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ServerData)	int32 MaximumPlayers;

protected:
	
	
	
};
