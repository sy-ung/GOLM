// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GOLMPlayerLabel.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMPlayerLabel : public UUserWidget
{
	GENERATED_BODY()

	UProgressBar *HealthBar;
	UTextBlock *PlayerNameText;

	class AGOLMCharacter *TrackingCharacter;

public:
	UFUNCTION(BlueprintCallable, Category = init)
		void SetPlayerLabels(UProgressBar *HealthProgressBar, UTextBlock *PlayerNameTextBlock);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config) float LifeTime;
	float Timer;

	void SetCharacterReference(class AGOLMCharacter *CurrentCharacter);

	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
};
