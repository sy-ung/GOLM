// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GOLMServerListWidget.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSessionToJoin
{
	GENERATED_USTRUCT_BODY()
	FOnlineSessionSearchResult *GameSession;
};

UCLASS()
class GOLM_API UGOLMServerListWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FSessionToJoin ServerSession;
public:

	void SetServerSession(FOnlineSessionSearchResult *ServerSession);


	UFUNCTION(BlueprintCallable, Category = ServerData)		FSessionToJoin GetServerSession();
	UFUNCTION(BlueprintCallable, Category = ServerData)		FString GetServerSessionName();
	UFUNCTION(BlueprintCallable, Category = ServerData)		int32 GetServerSessionPing();
	UFUNCTION(BlueprintCallable, Category = ServerData)		int32 GetServerSessionCurrentNumPlayers();
	UFUNCTION(BlueprintCallable, Category = ServerData)		int32 GetServerSessionMaxNumPlayers();
	UFUNCTION(BlueprintCallable, Category = SelectServer)	void Selected();
	
	

	
	
	
};
