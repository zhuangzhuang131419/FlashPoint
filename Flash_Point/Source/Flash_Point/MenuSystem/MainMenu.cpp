// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

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
	APlayerController* local = GetOwningPlayer();
	if (ensure(local)) {
		CreateWidget<UUserWidget>(local, CreateNewGameMenu);
		RemoveFromViewport();
	}
}

void UMainMenu::OnJoinGameClicked()
{
	APlayerController* local = GetOwningPlayer();
	if (ensure(local)) {
		CreateWidget<UUserWidget>(local, JoinGameMenu);
		RemoveFromViewport();
	}
}

void UMainMenu::OnLoadGameClicked()
{
	APlayerController* local = GetOwningPlayer();
	if (ensure(local)) {
		CreateWidget<UUserWidget>(local, LoadGameMenu);
		RemoveFromViewport();
	}
}

void UMainMenu::OnOptionClicked()
{
	APlayerController* local = GetOwningPlayer();
	if (ensure(local)) {
		CreateWidget<UUserWidget>(local, OptionMenu);
		RemoveFromViewport();
	}
}

void UMainMenu::OnExitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
