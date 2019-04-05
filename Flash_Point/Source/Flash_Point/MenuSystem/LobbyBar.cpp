// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyBar.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "MenuSystem/MapOverviewPanel.h"
#include "FlashPointGameInstance.h"
#include "Engine/World.h"

ULobbyBar::ULobbyBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

bool ULobbyBar::Initialize()
{
	if (Super::Initialize()) {
		if (ensure(JoinButton)) { JoinButton->OnClicked.AddDynamic(this, &ULobbyBar::OnJoinButton); }
		else { return false; }
		if (ensure(DummyBorder)) { DummyBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("OnDownDummyBorder")); }
		else { return false; }
		if (ensure(DummyBorder)) { DummyBorder->OnMouseDoubleClickEvent.BindUFunction(this, FName("OnDoubleClickDummyBorder")); }
		else { return false; }
		return true;
	}
	return false;
}

void ULobbyBar::InitializeAttributes(UMapOverviewPanel* inOverView, FGameLobbyInfo inInfo, int32 inIndex, int32 joinedPlayers)
{
	mapOverview = inOverView;
	lobbyInfo = inInfo;
	joinIndex = inIndex;
	capacity = joinedPlayers;
	if (!ensure(LobbyNameText)) return;
	if (!ensure(ModeText)) return;
	if (!ensure(JoinedPlayerText)) return;
	if (!ensure(InitialExplosionNum)) return;
	if (!ensure(InitialHazmatNum)) return;
	// show the lobby name text
	LobbyNameText->SetText(FText::FromString(inInfo.lobbyName));
	// display all the lobby info to the widget
	switch (inInfo.mode)
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
	// change the number of players text corresponding to info
	JoinedPlayerText->SetText(FText::FromString(FString::FromInt(joinedPlayers) + "/6"));
	// now do different displaying with regard to is saved
	if (inInfo.isSaved) {
		DisplayIsSaved(inInfo.isSaved);
		// change the detail text corresponding to info
		InitialExplosionNum->SetText(FText::FromString(FString::FromInt(lobbyInfo.lostVictimNum)));
		InitialHazmatNum->SetText(FText::FromString(FString::FromInt(lobbyInfo.boardHealth)));
	}
	else {
		// change the detail text corresponding to info
		InitialExplosionNum->SetText(FText::FromString(FString::FromInt(lobbyInfo.initialExplosions)));
		InitialHazmatNum->SetText(FText::FromString(FString::FromInt(lobbyInfo.initialHazmatNum)));
	}
}

void ULobbyBar::OnJoinButton()
{
	if (joinIndex >= 0 && capacity <= FPSESSION_STANDARD_SIZE) {
		// only join if the index is valid and not too many players are in the lobby
		UWorld* world = GetWorld();
		if (ensure(world)) {
			UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(world->GetGameInstance());
			if (ensure(gameInst)) {
				gameInst->JoinSessionOfIndex(joinIndex);
			}
		}
	}
}

void ULobbyBar::OnDownDummyBorder()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked Lobby Bar"));
	if (ensure(mapOverview)) {
		mapOverview->ShowMapInfo(lobbyInfo.map, "");
	}
}

void ULobbyBar::OnDoubleClickDummyBorder()
{
	UE_LOG(LogTemp, Warning, TEXT("Double Cliecked Lobby Bar"));
	// this is the same as clicking on join button
	OnJoinButton();
}
