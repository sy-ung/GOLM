// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMPlayerLabel.h"




void UGOLMPlayerLabel::SetPlayerLabels(UProgressBar *HealthProgressBar, UTextBlock *PlayerNameTextBlock)
{
	HealthBar = HealthProgressBar;
	PlayerNameText = PlayerNameTextBlock;
}

void UGOLMPlayerLabel::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGOLMPlayerLabel::SetCharacterReference(class AGOLMPlayerCharacter *CurrentCharacter)
{

}