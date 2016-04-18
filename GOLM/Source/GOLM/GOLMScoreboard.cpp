// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameState.h"
#include "GOLMScoreboard.h"



void UGOLMScoreboard::SetScoreList(UScrollBox *ScoreScrollBox)
{
	ScoreList = ScoreScrollBox;
}

void UGOLMScoreboard::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentNumOfScores = UGameplayStatics::GetGameState(GetWorld())->PlayerArray.Num();
	CreateScores();
}
void UGOLMScoreboard::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UGameplayStatics::GetGameState(GetWorld())->PlayerArray.Num() != CurrentNumOfScores)
	{
		CreateScores();
	}
}

void UGOLMScoreboard::NativeDestruct()
{
	Super::NativeDestruct();

}

void UGOLMScoreboard::CreateScores()
{
	AGameState *CurrentGameState = UGameplayStatics::GetGameState(GetWorld());
	ScoreList->ClearChildren();
	for (int32 i = 0; i < CurrentGameState->PlayerArray.Num(); i++)
	{
		UGOLMScoreboardItemWidget *NewScoreItem = CreateWidget<UGOLMScoreboardItemWidget>(GetWorld(), ScoreboardItemWidget.GetDefaultObject()->GetClass());
		AGOLMPlayerState *CurrentPlayerState = Cast<AGOLMPlayerState>(CurrentGameState->PlayerArray[i]);
		NewScoreItem->StorePlayerState(CurrentPlayerState);
		ScoreList->AddChild(NewScoreItem);
	}
}