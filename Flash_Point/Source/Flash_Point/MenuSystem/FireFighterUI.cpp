// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterUI.h"
#include "UObject/ConstructorHelpers.h"
#include "GameBoard.h"

UFireFighterUI::UFireFighterUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

void UFireFighterUI::NotifyDodge()
{
	PromptText->SetText(FText::FromString("Dodge The Fire!\n(Right Click To Cancel)"));
	YourTurnPrompt->SetVisibility(ESlateVisibility::Visible);
}

void UFireFighterUI::NotifyCommand()
{
	PromptText->SetText(FText::FromString("Accept the Command!\n(Right Click To Reject)"));
	YourTurnPrompt->SetVisibility(ESlateVisibility::Visible);
}

AGameBoard * UFireFighterUI::GetGameBoard()
{
	return gameBoard;
}

void UFireFighterUI::SetGameBoard(AGameBoard * inGameBoard)
{
	gameBoard = inGameBoard;
}

AFireFighterPawn * UFireFighterUI::GetRelatedPawn()
{
	return localPawn;
}

void UFireFighterUI::SetRelatedPawn(AFireFighterPawn * inPawn)
{
	localPawn = inPawn;
}

AFPPlayerController * UFireFighterUI::GetRelatedPlayer()
{
	return localPlayer;
}

void UFireFighterUI::SetRelatedPlayer(AFPPlayerController * inPlayer)
{
	localPlayer = inPlayer;
}

UFireFighterStatus * UFireFighterUI::GetPlayerStatusUIOf(int32 playerID)
{
	switch (playerID) {
	case 0:
		return P0Status;
		break;
	case 1:
		return P1Status;
		break;
	case 2:
		return P2Status;
		break;
	case 3:
		return P3Status;
		break;
	case 4:
		return P4Status;
		break;
	case 5:
		return P5Status;
		break;
	}

	return nullptr;
}

void UFireFighterUI::ShowCarrying(bool isCarrying)
{
	// TODO if have time do this setting with regard to the carried victim's actual looking
	if (isCarrying) {
		VictimIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		VictimIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFireFighterUI::ShowCarryHazmat(bool isCarrying)
{
	if (isCarrying) {
		HazmatIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		HazmatIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFireFighterUI::BindChatManagerWithUI(AChatManager * inMan)
{
	if (ensure(ChatBox)) {
		ChatBox->BindChatManager(inMan);
	}
}

void UFireFighterUI::RelateChatUIWithPlayer(AFPPlayerController * inPlayer)
{
	if (ensure(ChatBox)) {
		ChatBox->SetRelatedPlayer(inPlayer);
	}
}
