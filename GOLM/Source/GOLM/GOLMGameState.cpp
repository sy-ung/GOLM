// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerController.h"
#include "GOLMInGameHudWidget.h"
#include "GOLMKillWidget.h"
#include "GOLMGameMode.h"
#include "GOLMGameState.h"

AGOLMGameState::AGOLMGameState()
{

}



void AGOLMGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::MakeRandomColor(), PlayerArray[i]->GetName());*/
	}
}

void AGOLMGameState::CreateKillMessage(FName Killer, FName Victim)
{
	Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->InGameHUDReference->AddToKillBox(Killer,Victim);
	if (Role == ROLE_Authority)
		ClientCreateKillMessage(Killer, Victim);
}
void AGOLMGameState::ClientCreateKillMessage_Implementation(FName Killer, FName Victim)
{
	if (Role != ROLE_Authority)
	{
		CreateKillMessage(Killer, Victim);
	}
}