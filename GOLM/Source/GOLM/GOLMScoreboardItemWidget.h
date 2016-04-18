// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GOLMScoreboardItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMScoreboardItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UTextBlock *PlayerNameBlock;
	UTextBlock *KillBlock;
	UTextBlock *DeathBlock;
	UTextBlock *PingBlock;

	class AGOLMPlayerState *CurrentPlayerState;


public:

	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = Init)
	void SetScoreTextBlock(UTextBlock *NameTextBlock,UTextBlock *KillTextBlock,UTextBlock *DeathTextBlock,UTextBlock *PingTextBlock);

	void StorePlayerState(class AGOLMPlayerState *NewPlayerState);
	
	void UpdateScoreDisplay();
	
};
