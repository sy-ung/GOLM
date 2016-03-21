// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"


#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GOLMMouseWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API UGOLMMouseWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UImage *MouseCursor;
	

public:
	
	
	UFUNCTION(BlueprintCallable,Category = Init)
	void SetMouseReference(UImage *MouseReference);

	UFUNCTION(BlueprintCallable,Category = MouseStuff)
	UImage *GetMouseReference();

	void MoveMouseCursor(class AGOLMPlayerController *PlayerController);

	virtual void NativeConstruct() override;
	

	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
	
};
