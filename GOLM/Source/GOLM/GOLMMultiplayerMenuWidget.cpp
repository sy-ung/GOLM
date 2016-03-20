// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMMultiplayerMenuWidget.h"

void UGOLMMultiplayerMenuWidget::AddServer(FOnlineSessionSearchResult *NewSession, UWorld *World)
{
	if (NewSession->IsValid())
	{
		if (ServerListWidget != NULL)
		{
			if(World != NULL)
			{
				UGOLMServerListWidget *NewServerWidget = CreateWidget<UGOLMServerListWidget>(World, ServerListWidget);
				if(NewServerWidget != NULL)
				{
					NewServerWidget->SetServerSession(NewSession);
					FServerListItemWidget NewServer;
					NewServer.ServerItemWidget = NewServerWidget;
					if (ServerScrollBox != NULL)
					{
						ServerScrollBox->AddChild(NewServerWidget);
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "Server Scroll box is NULL for " + this->GetName() );
					}
					//Servers.Add(NewServer);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, "NewServerWidget is NULL");
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, "World is NULL");
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, "SERVER LIST WIDGET IS NULL");
		}
	}

}

TArray<FServerListItemWidget> UGOLMMultiplayerMenuWidget::GetServers()
{
	return Servers;
}

void UGOLMMultiplayerMenuWidget::SetServerScrollBox(UScrollBox *ServerScrollBox)
{
	this->ServerScrollBox = ServerScrollBox;
}

