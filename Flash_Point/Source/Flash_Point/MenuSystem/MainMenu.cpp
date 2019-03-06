// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

bool UMainMenu::Initialize() {
	if (Super::Initialize()) {
		if (ensure(New_Game)) { New_Game->OnClicked.AddDynamic(this, &UMainMenu::OnNewGameClicked); }
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
