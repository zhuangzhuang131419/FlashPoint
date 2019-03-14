// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatWidget.h"
#include "ChatManager.h"
#include "FPPlayerController.h"


void UChatWidget::BindChatManager(AChatManager* inChatMan) {
	chatMan = inChatMan;
	if (ensure(chatMan)) {
		chatMan->SetChatWidget(this);
	}
}

void UChatWidget::SendMessage(FString message)
{
	if (!ensure(localPlayer)) return;
	FString messageWithID = localPlayer->GetPlayerName().Append(": ").Append(message);
	if (chatMan->HasAuthority()) {
		chatMan->SendGlobalText(messageWithID);
	}
	else {
		localPlayer->ServerSendGlobalText(chatMan, messageWithID);
	}
}

void UChatWidget::SetRelatedPlayer(AFPPlayerController * inPlayer)
{
	localPlayer = inPlayer;
}
