// Fill out your copyright notice in the Description page of Project Settings.

#include "SavedGameBar.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "MenuSystem/MapOverviewPanel.h"
#include "FlashPointGameInstance.h"
#include "Engine/World.h"

USavedGameBar::USavedGameBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

bool USavedGameBar::Initialize() {
	if (Super::Initialize()) {
		if (ensure(LoadGameButton)) { LoadGameButton->OnClicked.AddDynamic(this, &USavedGameBar::OnLoadButton); }
		else { return false; }
		if (ensure(DummyBorder)) { DummyBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("OnDownDummyBorder")); }
		else { return false; }
		if (ensure(DummyBorder)) { DummyBorder->OnMouseDoubleClickEvent.BindUFunction(this, FName("OnDoubleClickDummyBorder")); }
		else { return false; }
		return true;
	}
	return false;
}

void USavedGameBar::InitializeAttributes(UMapOverviewPanel * inOverView, FMapSaveInfo inInfo)
{
	mapOverview = inOverView;
	savedInfo = inInfo;
	// Setting the bar according to specified info
	if (!ensure(SavedGameText)) return;
	if (!ensure(DateText)) return;
	if (!ensure(ModeText)) return;
	if (!ensure(LostVictimsText)) return;
	if (!ensure(BoardHealthText)) return;
	SavedGameText->SetText(FText::FromString(savedInfo.saveName));
	DateText->SetText(FText::FromString(savedInfo.saveDate.Left(10)));
	switch (savedInfo.boardInfo.gameModeType)
	{
	case EGameType::Family:
		ModeText->SetText(FText::FromString("Family"));
		break;
	case EGameType::Experienced:
		ModeText->SetText(FText::FromString("Experienced"));
		break;
	case EGameType::Betryal:
		ModeText->SetText(FText::FromString("Betryal"));
		break;
	default:
		break;
	}
	LostVictimsText->SetText(FText::FromString(FString::FromInt(savedInfo.boardInfo.victimLostNum)));
	BoardHealthText->SetText(FText::FromString(FString::FromInt(savedInfo.boardInfo.currentHealth)));
}

void USavedGameBar::OnLoadButton()
{
	// load the game corresponding the specified information
	UWorld* world = GetWorld();
	if (ensure(world)) {
		UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(world->GetGameInstance());
		if (ensure(gameInst)) {
			gameInst->CreateSavedLobby(savedInfo);
		}
	}
}

void USavedGameBar::OnDownDummyBorder()
{
	// show information about the saved game to the map overview Panel
	if (!ensure(mapOverview)) return;
	mapOverview->ShowMapInfo(savedInfo.map, "");
}

void USavedGameBar::OnDoubleClickDummyBorder()
{
	// same as on click load map
	OnLoadButton();
}
