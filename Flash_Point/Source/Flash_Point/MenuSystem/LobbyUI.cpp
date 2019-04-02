// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyUI.h"
#include "CrewManager.h"
#include "LobbyManager.h"
#include "MenuSystem/SwitchRoleUI.h"
#include "MenuSystem/ChatWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

ULobbyUI::ULobbyUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

bool ULobbyUI::Initialize()
{
	bool success = Super::Initialize();
	if (success) {
		if (!ensure(ReadyOrStartButton)) return false;
		ReadyOrStartButton->OnClicked.AddDynamic(this, &ULobbyUI::OnReadyOrStart);
	}
	return success;
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

void ULobbyUI::BindLobbyManagerWithUI(ALobbyManager * inMan)
{
	lobbyMan = inMan;
}

void ULobbyUI::RelateChatUIWithPlayer(AFPPlayerController * inPlayer)
{
	if (ensure(ChatPanel)) {
		ChatPanel->SetRelatedPlayer(inPlayer);
	}
}

void ULobbyUI::ChangeJoinStartButtonStatus(FString inMessage, bool isEnabled)
{
	if (ensure(JOSButtonText) && ensure(ReadyOrStartButton)) {
		JOSButtonText->SetText(FText::FromString(inMessage));
		ReadyOrStartButton->SetIsEnabled(isEnabled);
	}
}

void ULobbyUI::OnReadyOrStart()
{
	// this function acts differently between client and server
	if (ensure(lobbyMan)) {
		lobbyMan->PlayerReadyStart();
	}
}
