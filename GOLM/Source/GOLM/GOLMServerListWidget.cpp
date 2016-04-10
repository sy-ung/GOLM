// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMServerListWidget.h"
#include "GOLMGameInstance.h"

void UGOLMServerListWidget::SetServerSession(FOnlineSessionSearchResult *ServerSession)
{
	this->ServerSession.GameSession = ServerSession;
}

FSessionToJoin UGOLMServerListWidget::GetServerSession()
{
	return ServerSession;
}

FString UGOLMServerListWidget::GetServerSessionName()
{
	return ServerSession.GameSession->Session.OwningUserName;
}

int32 UGOLMServerListWidget::GetServerSessionPing()
{
	return ServerSession.GameSession->PingInMs;
}

int32 UGOLMServerListWidget::GetServerSessionCurrentNumPlayers()
{
	return ServerSession.GameSession->Session.SessionSettings.NumPublicConnections -
		ServerSession.GameSession->Session.NumOpenPublicConnections;
}

int32 UGOLMServerListWidget::GetServerSessionMaxNumPlayers()
{
	return (ServerSession.GameSession->Session.SessionSettings.NumPublicConnections);
}

void UGOLMServerListWidget::Selected()
{
	Cast<UGOLMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetSelectedSession(ServerSession);
}