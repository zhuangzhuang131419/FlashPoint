// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MapOverviewPanel.h"
#include "MenuSystem/LobbyBar.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "FlashPointGameInstance.h"
#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> LobbyBarClassFinder(TEXT("/Game/BPs/WidgetComponents/WBP_LobbyBar"));
	if (!ensure(LobbyBarClassFinder.Class))	return;
	LobbyBarClass = LobbyBarClassFinder.Class;
}

void UMainMenu::ClearAllLobbyList()
{
	if (ensure(LobbyArrangement)) {
		// get rid of all children
		LobbyArrangement->ClearChildren();
	}
	// also clear the map overview
	if (ensure(JoinMapOverview)) {
		JoinMapOverview->ClearMapInfo();
	}
}

void UMainMenu::InsertLobbyBar(FGameLobbyInfo inInfo, int32 inIndex, int32 joinedPlayers)
{
	// create a new lobby bar with specified informations
	if (!ensure(LobbyBarClass)) return;
	if (!ensure(LobbyArrangement)) return;
	if (!ensure(gameInst)) return;
	if (inInfo.lobbyName.IsEmpty()) return;
	ULobbyBar* tempLobbyBar = CreateWidget<ULobbyBar>(gameInst, LobbyBarClass);
	if (ensure(tempLobbyBar)) {
		tempLobbyBar->InitializeAttributes(JoinMapOverview, inInfo, inIndex, joinedPlayers);
		LobbyArrangement->AddChild(tempLobbyBar);
	}
}

void UMainMenu::ShowRefreshing(bool isRefreshing)
{
	if (ensure(RefreshingText)) {
		RefreshingText->SetText(FText::FromString("Refreshing Lobby List"));
		if (isRefreshing) {
			RefreshingText->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			RefreshingText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMainMenu::ShowJoinStatus(bool hasFailed)
{
	if (!ensure(RefreshingText)) return;
	if (hasFailed) {
		RefreshingText->SetText(FText::FromString("Failed to join lobby"));
	}
	else {
		RefreshingText->SetText(FText::FromString("Joining"));
	}
	RefreshingText->SetVisibility(ESlateVisibility::Visible);
}

bool UMainMenu::Initialize() {
	if (Super::Initialize()) {
		if (ensure(New_Game)) { New_Game->OnClicked.AddDynamic(this, &UMainMenu::OnNewGameClicked); }
		else { return false; }
		if (ensure(Join_Game)) { Join_Game->OnClicked.AddDynamic(this, &UMainMenu::OnJoinGameClicked); }
		else { return false; }
		if (ensure(Load_Game)) { Load_Game->OnClicked.AddDynamic(this, &UMainMenu::OnLoadGameClicked); }
		else { return false; }
		if (ensure(Option_Button)) { Option_Button->OnClicked.AddDynamic(this, &UMainMenu::OnOptionClicked); }
		else { return false; }
		if (ensure(Exit_Game)) { Exit_Game->OnClicked.AddDynamic(this, &UMainMenu::OnExitClicked); }
		else { return false; }
		if (ensure(RefreshLobbies)) { RefreshLobbies->OnClicked.AddDynamic(this, &UMainMenu::OnRefreshLobbies); }
		else { return false; }
		return true;
	}
	return false;
}

void UMainMenu::OnNewGameClicked()
{
	
}

void UMainMenu::OnJoinGameClicked()
{
	if (ensure(gameInst)) {
		gameInst->RefreshLobbyList();
	}
}

void UMainMenu::OnLoadGameClicked()
{
	// TODO refresh the saved games in order for player to select
}

void UMainMenu::OnOptionClicked()
{
	
}

void UMainMenu::OnExitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::OnRefreshLobbies()
{
	if (ensure(gameInst)) {
		gameInst->RefreshLobbyList();
	}
}
