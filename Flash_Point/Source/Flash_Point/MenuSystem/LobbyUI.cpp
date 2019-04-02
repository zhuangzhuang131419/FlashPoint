// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyUI.h"
#include "CrewManager.h"
#include "MenuSystem/SwitchRoleUI.h"
#include "MenuSystem/ChatWidget.h"

ULobbyUI::ULobbyUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

void ULobbyUI::BindChatManagerWithUI(AChatManager * inMan)
{
	if (ensure(ChatPanel)) {
		ChatPanel->BindChatManager(inMan);
	}
}

void ULobbyUI::BindCrewManagerWithUI(ACrewManager * inMan)
{
	crewMan = inMan;
	if (ensure(SelectRolePanel) && ensure(inMan)) {
		SelectRolePanel->BindCrewManger(inMan);
		inMan->SetCrewChangeUI(SelectRolePanel);
	}
}

void ULobbyUI::RelateChatUIWithPlayer(AFPPlayerController * inPlayer)
{
	if (ensure(ChatPanel)) {
		ChatPanel->SetRelatedPlayer(inPlayer);
	}
}
