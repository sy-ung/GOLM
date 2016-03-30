// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMCharacter.h"
#include "GOLMInGameHudWidget.h"

void UGOLMInGameHudWidget::MapZoom(float value)
{
	FVector2D OriginalSize = MapCanvas->GetSize();
	FVector2D NewSize = FVector2D(OriginalSize.X + value, OriginalSize.Y + value);
	MapCanvas->SetSize(NewSize);
	MapCanvas->SetPosition(FVector2D(NewSize.X / -2, NewSize.Y / -2));
}

void UGOLMInGameHudWidget::UpdateMap()
{

}


