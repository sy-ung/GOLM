// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLM_HUD.h"

AGOLM_HUD::AGOLM_HUD()
{

}

void AGOLM_HUD::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AGOLMPlayerController>(GetOwningPlayerController());
	PlayerState = Cast<AGOLMPlayerState>(PlayerController->PlayerState);
}

void AGOLM_HUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AGOLM_HUD::DrawHUD()
{
	//Super::DrawHUD();
}
