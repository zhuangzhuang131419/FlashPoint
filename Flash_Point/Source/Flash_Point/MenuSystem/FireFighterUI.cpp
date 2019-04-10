// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterUI.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "GameBoard.h"
#include "SavePanel.h"
#include "FlashPointGameInstance.h"

UFireFighterUI::UFireFighterUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<USavePanel> SavePanelClassFinder(TEXT("/Game/BPs/WidgetComponents/WBP_save"));
	if (!ensure(SavePanelClassFinder.Class)) return;
	SavePanelClass = SavePanelClassFinder.Class;
}

void UFireFighterUI::PromptCommandStatus(EAcceptanceStatus acceptState)
{
	if (!ensure(PromptText) || !ensure(YourTurnPrompt) || !ensure(ConfirmTurnButton)) return;
	YourTurnPrompt->SetVisibility(ESlateVisibility::Visible);
	switch (acceptState)
	{
	case EAcceptanceStatus::Accepted:
		// if the command is already accepted enable the operations panels
		ConfirmTurnButton->SetIsEnabled(true);
		EnableOperationPanels(true);
		PromptText->SetText(FText::FromString("Command accepted!"));
		break;
	case EAcceptanceStatus::Rejected:
		// if the command is already rejected enable the operations panels
		ConfirmTurnButton->SetIsEnabled(true);
		EnableOperationPanels(true);
		PromptText->SetText(FText::FromString("Command rejected!"));
		break;
	case EAcceptanceStatus::Waiting:
		// if the opereation is just sent, disable the operation panels
		ConfirmTurnButton->SetIsEnabled(false);
		EnableOperationPanels(false);
		PromptText->SetText(FText::FromString("Waiting for response..."));
		break;
	case EAcceptanceStatus::Empty:
		break;
	default:
		break;
	}
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
	if (!ensure(VictimIcon)) return;
	if (isCarrying) {
		VictimIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		VictimIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFireFighterUI::ShowCarryHazmat(bool isCarrying)
{
	if (!ensure(HazmatIcon)) return;
	if (isCarrying) {
		HazmatIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		HazmatIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFireFighterUI::ShowLeading(bool isCarrying)
{
	if (!ensure(LeadingIcon)) return;
	if (isCarrying) {
		LeadingIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		LeadingIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFireFighterUI::EnableOperationPanels(bool enable)
{
	if (!ensure(AbilityBar) || !ensure(InventoryAndSkip)) return;
	AbilityBar->SetIsEnabled(enable);
	InventoryAndSkip->SetIsEnabled(enable);
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

void UFireFighterUI::SetBeginTurnNotify(bool isBegin)
{
	isBeginOfTurn = isBegin;
}

void UFireFighterUI::NotifySomeOneLeft()
{
	isNotifyPlayerLeave = true;
	if (ensure(YourTurnPrompt) && ensure(PromptText) && ensure(ConfirmTurnButton)) {
		YourTurnPrompt->SetVisibility(ESlateVisibility::Visible);
		PromptText->SetText(FText::FromString("Some has left the game, game has to stop..."));
		ConfirmTurnButton->SetIsEnabled(true);
	}
}

void UFireFighterUI::NotifyPlaceCar(FString promptInfo)
{
	if (ensure(YourTurnPrompt) && ensure(PromptText) && ensure(ConfirmTurnButton)) {
		isBeginOfTurn = false;
		YourTurnPrompt->SetVisibility(ESlateVisibility::Visible);
		PromptText->SetText(FText::FromString(promptInfo));
		ConfirmTurnButton->SetIsEnabled(true);
	}
}

bool UFireFighterUI::Initialize()
{
	bool success = Super::Initialize();
	if (success) {
		if (!ensure(ConfirmTurnButton)) return false;
		ConfirmTurnButton->OnClicked.AddDynamic(this, &UFireFighterUI::NotifyCrewChange);
		if (!ensure(SaveButton)) return false;
		SaveButton->OnClicked.AddDynamic(this, &UFireFighterUI::OnSaveClicked);
		if (!ensure(ExitButton)) return false;
		ExitButton->OnClicked.AddDynamic(this, &UFireFighterUI::OnExitClicked);
		if (!ensure(EndGameQuit)) return false;
		EndGameQuit->OnClicked.AddDynamic(this, &UFireFighterUI::OnEndGameExit);
	}
	return success;
}

void UFireFighterUI::NotifyCrewChange()
{
	if (!ensure(localPlayer)) return;
	if (ensure(localPawn)) {
		if (localPawn->GetFireFighterRole() == ERoleType::FireCaptain && localPlayer->GetCurrentOperation() == EGameOperations::Command) {
			// for starting of the game this is not applicapable
			localPlayer->ServerSetCommandStatus(localPawn, EAcceptanceStatus::Empty);
		}
	}
	if (isNotifyPlayerLeave) {
		localPlayer->HostEndGame();
		localPlayer->ClientTravel("/Game/maps/MainMenu", ETravelType::TRAVEL_Absolute);
		localPlayer->LeaveGameSession();
	}
	// if not the begining of turn, this is just a prompt to notify events
	if (!isBeginOfTurn) return;

	// if it is beginning of the turn, enable operations
	EnableOperationPanels(true);
	if (ensure(localPlayer)) {
		UE_LOG(LogTemp, Warning, TEXT("Notified Crew Change"));
		localPlayer->PromtCrewChange();
	}
}

void UFireFighterUI::OnExitClicked()
{
	// first disable the button as it is clicked
	if (!ensure(ExitButton)) return;
	if (!ensure(gameBoard)) return;
	ExitButton->SetIsEnabled(false);
	EnableOperationPanels(false);
	// notify there's some player leaving
	gameBoard->LeaveBoard(localPawn);
}

void UFireFighterUI::OnSaveClicked()
{
	// when clicekd on save button show the save game panel
	if (!saveGamePanel) {
		if (ensure(SavePanelClass)) {
			saveGamePanel = CreateWidget<USavePanel>(localPlayer, SavePanelClass);
			if (ensure(saveGamePanel)) {
				saveGamePanel->AddToViewport();
			}
		}
	}
	else {
		saveGamePanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFireFighterUI::OnEndGameExit()
{
	if (!ensure(localPlayer)) return;
	localPlayer->HostEndGame();
	localPlayer->ClientTravel("/Game/maps/MainMenu", ETravelType::TRAVEL_Absolute);
	localPlayer->LeaveGameSession();
}
