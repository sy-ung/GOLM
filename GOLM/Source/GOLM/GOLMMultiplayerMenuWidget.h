// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMServerListWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GOLMMultiplayerMenuWidget.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FServerListItemWidget
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Servers)
	UGOLMServerListWidget *ServerItemWidget;

};


UCLASS()
class GOLM_API UGOLMMultiplayerMenuWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UScrollBox *ServerScrollBox;
public:

	void AddServer(FOnlineSessionSearchResult *NewSession, UWorld *World);
	void ClearServers();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ServerListWidget)
		TSubclassOf<UUserWidget> ServerListWidget;

	UFUNCTION(BlueprintCallable, Category = ServerScrollBoxData)
		void SetServerScrollBox(UScrollBox *ServerScrollBox);

		
};
