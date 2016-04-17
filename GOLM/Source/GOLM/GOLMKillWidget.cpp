// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMKillWidget.h"

void UGOLMKillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KillerColor = FLinearColor::MakeRandomColor();
	VictimColor = FLinearColor::MakeRandomColor();

	KillerTextBlock->SetColorAndOpacity(KillerColor);
	KillerTextBlock->SetText(FText::FromString(Killer.ToString()));
	VictimTextBlock->SetColorAndOpacity(VictimColor);
	VictimTextBlock->SetText(FText::FromString(Victim.ToString()));
}
void UGOLMKillWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (lifetimer >= lifetime)
	{
		RemoveFromRoot();
		RemoveFromParent();
	}
	else
	{
		lifetimer += InDeltaTime;
		if (lifetimer >= FadeTimeStart)
		{
			FadeTimer += InDeltaTime;
			float Alpha = 1 - (FadeTimer / (lifetime - FadeTimeStart));
			KillerTextBlock->SetColorAndOpacity(FLinearColor(KillerColor.R, KillerColor.G, KillerColor.B, Alpha));
			VictimTextBlock->SetColorAndOpacity(FLinearColor(VictimColor.R, VictimColor.G, VictimColor.B, Alpha));
			BodyTextBlock->SetColorAndOpacity(FLinearColor(1, 1, 1, Alpha));
		}
	}
}

void UGOLMKillWidget::SetKillMessage(FName KillerName, FName VictimName)
{
	Killer = KillerName;
	Victim = VictimName;
}

void UGOLMKillWidget::SetKillerVictimTextBlock(UTextBlock *KillerText, UTextBlock *VictimText, UTextBlock *BodyText)
{
	KillerTextBlock = KillerText;
	VictimTextBlock = VictimText;
	BodyTextBlock = BodyText;
}

