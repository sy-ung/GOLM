// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMMiniMapCamera.h"
#include "GOLMCharacter.h"

void AGOLMMiniMapCamera::UpdateCamera()
{
	if (PlayerCharacter != NULL)
	{
		FVector PlayerLoc = PlayerCharacter->GetActorLocation();
		SetActorLocation(FVector(PlayerLoc.X, PlayerLoc.Y, FMath::Lerp<float>(GetActorLocation().Z, Height, 0.25)));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "PlayerChar is NULL");

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::SanitizeFloat(Height));
}

void AGOLMMiniMapCamera::SetPlayerCharacter(AGOLMCharacter *Player)
{
	PlayerCharacter = Player;
}

void AGOLMMiniMapCamera::Zoom(float value)
{
	Height += value;
	Height = FMath::Clamp<float>(Height, 3000, 10000);
	
}
