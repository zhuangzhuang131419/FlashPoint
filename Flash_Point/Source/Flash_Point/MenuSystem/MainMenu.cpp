// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> NewGameMenuFinder(TEXT("/Game/BPs/Widgets/WBP_CreateNewGameUI"));
	if (!ensure(NewGameMenuFinder.Class))	return;
	CreateNewGameMenu = NewGameMenuFinder.Class;
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
		return true;
	}
	return false;
}

void UMainMenu::OnNewGameClicked()
{
	
}

void UMainMenu::OnJoinGameClicked()
{
	// TODO refresh the server list in order to display to the player
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
