// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMScoreboardItemWidget.h"
#include "GOLMScoreboard.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMScoreboard : public UUserWidget
{
	GENERATED_BODY()

	UScrollBox *ScoreList;
	int32 CurrentNumOfScores;
public:
	UFUNCTION(BlueprintCallable, Category = Init)
		void SetScoreList(UScrollBox *ScoreScrollBox);

	void CreateScores();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemWidget) TSubclassOf<UUserWidget> ScoreboardItemWidget;
};
