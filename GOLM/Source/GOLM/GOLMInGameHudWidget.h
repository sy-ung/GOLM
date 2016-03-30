// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


#include "GOLMInGameHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMInGameHudWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MapData) UImage *MapImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MapData) UCanvasPanelSlot *MapCanvas;

	UFUNCTION(BlueprintCallable, Category = MapFunction) void UpdateMap();
	UFUNCTION(BlueprintCallable, Category = MapFunction) void MapZoom(float value);
	
	
};
