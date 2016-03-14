// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOLMCharacter.h"
#include "GOLMPlayerController.h"
#include "GOLMPlayerState.h"
#include "GOLMGameState.h"
#include "GameFramework/HUD.h"
#include "GOLM_HUD.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API AGOLM_HUD : public AHUD
{

	GENERATED_BODY()
private:
	AGOLMPlayerController *PlayerController;
	AGOLMPlayerState *PlayerState;
	AGOLMGameState *GameState;
public:


	AGOLM_HUD();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void DrawHUD() override;

};
