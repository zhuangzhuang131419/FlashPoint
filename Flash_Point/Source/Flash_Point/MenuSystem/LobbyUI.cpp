// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyUI.h"
#include "MenuSystem/ChatWidget.h"

ULobbyUI::ULobbyUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

void ULobbyUI::BindChatManagerWithUI(AChatManager * inMan)
{
	if (ensure(ChatPanel)) {
		ChatPanel->BindChatManager(inMan);
	}
}

void ULobbyUI::RelateChatUIWithPlayer(AFPPlayerController * inPlayer)
{
	if (ensure(ChatPanel)) {
		ChatPanel->SetRelatedPlayer(inPlayer);
	}
}
