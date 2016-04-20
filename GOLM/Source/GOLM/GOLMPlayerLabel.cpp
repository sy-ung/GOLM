// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMPlayerLabel.h"


void UGOLMPlayerLabel::NativeConstruct()
{
	Super::NativeConstruct();
	FTimerHandle TimeHandle;
	FTimerDynamicDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "DisplayLabels");
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetWorldTimerManager().SetTimer(TimeHandle,TimerDelegate, 0.2f, true,0.2f);
}

void UGOLMPlayerLabel::SetPlayerLabels(UProgressBar *HealthProgressBar, UTextBlock *PlayerNameTextBlock)
{
	HealthBar = HealthProgressBar;
	PlayerNameText = PlayerNameTextBlock;
}

void UGOLMPlayerLabel::DisplayLabels()
{
	if (TrackingCharacter != NULL)
	{
		if (TrackingCharacter->PlayerState != NULL)
		{

			if (Cast<AGOLMPlayerState>(TrackingCharacter->PlayerState)->bIsKyle)
			{
				PlayerNameText->SetColorAndOpacity(FLinearColor::MakeRandomColor());
			}
			else if (TrackingCharacter->IsLocallyControlled())
			{
				PlayerNameText->SetColorAndOpacity(FLinearColor(FColor::Cyan));
			}
			else
			{
				PlayerNameText->SetColorAndOpacity(FLinearColor::Red);
			}

			PlayerNameText->SetText(FText::FromString(TrackingCharacter->PlayerState->PlayerName));
		}

		HealthBar->SetPercent(TrackingCharacter->Health / 100.0f);
	}
}

void UGOLMPlayerLabel::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//DisplayLabels();


}

void UGOLMPlayerLabel::SetCharacterReference(class AGOLMCharacter *CurrentCharacter)
{
	TrackingCharacter = CurrentCharacter;
}