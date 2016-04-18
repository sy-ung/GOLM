// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "Blueprint/UserWidget.h"
#include "GOLMPlayerState.h"
#include "GOLMGameState.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API AGOLMGameState : public AGameState
{
	GENERATED_BODY()

public:
	AGOLMGameState();

	virtual void Tick(float DeltaSeconds) override;
	
	void CreateKillMessage(FName Killer, FName Victim);

	UFUNCTION(Client, Unreliable, NetMulticast)
		void ClientCreateKillMessage(FName Killer, FName Victim);
		void ClientCreateKillMessage_Implementation(FName Killer, FName Victim);


	
	
};
