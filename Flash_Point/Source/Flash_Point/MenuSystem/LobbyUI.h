// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralTypes.h"
#include "LobbyUI.generated.h"

class ACrewManager;
class AChatManager;
class AFPPlayerController;
class UChatWidget;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULobbyUI(const FObjectInitializer& ObjectInitializer);

	// FUNCTIONS
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Lobby Creation")
	void ShowAllLobbyInfo(FGameLobbyInfo inInfo);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Lobby Creation")
	void ShowSelectedRole(ERoleType inRole);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Lobby Creation")
	void UpdateDisplayedInfo(FPlayerLobbyInfo P0, FPlayerLobbyInfo P1, FPlayerLobbyInfo P2, FPlayerLobbyInfo P3, FPlayerLobbyInfo P4, FPlayerLobbyInfo P5);
	void BindChatManagerWithUI(AChatManager* inMan);
	void BindCrewManagerWithUI(ACrewManager* inMan);
	void RelateChatUIWithPlayer(AFPPlayerController* inPlayer);
	
protected:
	// Managers related to the lobby UI
	UPROPERTY(BlueprintReadWrite, Category = "Lobby Creation")
	ACrewManager* crewMan;
	
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UChatWidget* ChatPanel = nullptr;
	
};
