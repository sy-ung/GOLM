// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMGameInstance.h"
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

void UGOLMMultiplayerMenuWidget::ClearServers()
{
	if (ServerScrollBox != NULL)
	{
		ServerScrollBox->ClearChildren();
	}
}


void UGOLMMultiplayerMenuWidget::SetServerScrollBox(UScrollBox *ServerScrollBox)
{
	this->ServerScrollBox = ServerScrollBox;
}

bool UGOLMMultiplayerMenuWidget::CheckPlayerName(FText PlayerNameCheck)
{
	FString Test = PlayerNameCheck.ToString();
	
	StatusTextBox->SetColorAndOpacity(FLinearColor::Yellow);

	if (Test.GetCharArray().Num() > 15 || Test.GetCharArray().Num() <= 0)
	{
		StatusTextBox->SetText(FText::FromString("Name is too long or too short. Must be 1-15 Characters"));
		return false;
	}

	for (int32 i = 0; i < Test.GetCharArray().Num() - 1; i++)
	{
		if (Test.GetCharArray()[i] != ' ')
		{

			Cast<UGOLMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->CurrentPlayerName = FName(*PlayerNameCheck.ToString());	
			StatusTextBox->SetText(FText::FromString(""));
			return true;
		}
	}

	StatusTextBox->SetText(FText::FromString("Name cannot be blank"));
	return false;
	
}