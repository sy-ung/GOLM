// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerState.h"
#include "GOLMScoreboardItemWidget.h"


void UGOLMScoreboardItemWidget::SetScoreTextBlock(UTextBlock *NameTextBlock, UTextBlock *KillTextBlock, UTextBlock *DeathTextBlock, UTextBlock *PingTextBlock)
{
	PlayerNameBlock = NameTextBlock;
	KillBlock = KillTextBlock;
	DeathBlock = DeathTextBlock;
	PingBlock = PingTextBlock;
}

void  UGOLMScoreboardItemWidget::StorePlayerState(AGOLMPlayerState *NewPlayerState)
{
	CurrentPlayerState = NewPlayerState;
}

void UGOLMScoreboardItemWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateScoreDisplay();
}

void UGOLMScoreboardItemWidget::UpdateScoreDisplay()
{
	PlayerNameBlock->SetColorAndOpacity(FLinearColor::Green);
	PlayerNameBlock->SetText(FText::FromString(CurrentPlayerState->PlayerName));

	KillBlock->SetColorAndOpacity(FLinearColor::Red);
	KillBlock->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->PlayerKills)));

	DeathBlock->SetColorAndOpacity(FLinearColor::Yellow);
	DeathBlock->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->PlayerDeath)));

	PingBlock->SetColorAndOpacity(FLinearColor::Gray);
	PingBlock->SetText(FText::FromString(FString::SanitizeFloat(CurrentPlayerState->Ping)));
}
