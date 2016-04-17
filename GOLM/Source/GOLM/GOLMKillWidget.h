// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GOLMKillWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMKillWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FName Killer;
	FName Victim;

	UTextBlock *KillerTextBlock;
	UTextBlock *VictimTextBlock;
	UTextBlock *BodyTextBlock;

	FLinearColor KillerColor;
	FLinearColor VictimColor;

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;


	float lifetimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Data)	
		float lifetime;

	float FadeTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
		float FadeTimeStart;
	


	void SetKillMessage(FName KillerName, FName VictimName);

	UFUNCTION(BlueprintCallable, Category = Init) void SetKillerVictimTextBlock(UTextBlock *KillerText, UTextBlock *VictimText, UTextBlock *BodyText);

};
