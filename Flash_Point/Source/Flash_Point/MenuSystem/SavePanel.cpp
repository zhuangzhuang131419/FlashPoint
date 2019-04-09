// Fill out your copyright notice in the Description page of Project Settings.

#include "SavePanel.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "FPPlayerController.h"

USavePanel::USavePanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

bool USavePanel::Initialize()
{
	bool success = Super::Initialize();
	if (success) {
		if (ensure(YesButton)) { YesButton->OnClicked.AddDynamic(this, &USavePanel::OnYesButtonClicked); }
		else { return false; }
		if (ensure(CancelButton)) { CancelButton->OnClicked.AddDynamic(this, &USavePanel::OnCancelButtonClicked); }
		else { return false; }
	}
	return success;
}

void USavePanel::OnYesButtonClicked()
{
	// when clicked on the save button save game using game instance
	UWorld* world = GetWorld();
	if (!ensure(SavedGameName)) return;
	if (!ensure(world)) return;
	AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
	if (ensure(localPlayer)) {
		localPlayer->SaveCurrentGame(SavedGameName->GetText().ToString());
	}
	SetVisibility(ESlateVisibility::Collapsed);
}

void USavePanel::OnCancelButtonClicked()
{
	// as cancel is clicked, hide this widget
	SetVisibility(ESlateVisibility::Collapsed);
}
